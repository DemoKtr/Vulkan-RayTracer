#pragma once
#include <vector>
#include <iostream>
#include <cassert>
#include <list>


namespace memory {

    namespace AeraMemoryPool {
        struct MemoryPoolInput {
            int count;
            std::vector<size_t> number_of_chunks;
            std::vector<size_t> size_of_chunk;
        };

        class MemoryPool {
        private:
            struct FreeBlock {
                FreeBlock* next;
            };

            struct SizeClass {
                size_t chunkSize;          // Rozmiar pojedynczego chunku
                size_t numChunks;          // Liczba chunków
                void* memory;              // Pula pamięci
                FreeBlock* freeList;       // Lista wolnych bloków

                SizeClass(size_t chunkSize, size_t numChunks)
                    : chunkSize(chunkSize), numChunks(numChunks), freeList(nullptr) {
                    // Alokacja pamięci dla całej klasy
                    memory = operator new(chunkSize * numChunks);

                    // Inicjalizacja listy wolnych bloków
                    for (size_t i = 0; i < numChunks; ++i) {
                        void* chunk = reinterpret_cast<void*>(
                            reinterpret_cast<uint8_t*>(memory) + i * chunkSize);
                        auto* block = reinterpret_cast<FreeBlock*>(chunk);
                        block->next = freeList;
                        freeList = block;
                    }
                }

                ~SizeClass() {

                }

                void ClearMemory() {
                    operator delete(memory);
                }

                void* Allocate() {
                    if (!freeList) {
                        throw std::bad_alloc();
                    }
                    void* chunk = freeList;
                    freeList = freeList->next;
                    return chunk;
                }

                void Deallocate(void* chunk) {
                    auto* block = reinterpret_cast<FreeBlock*>(chunk);
                    block->next = freeList;
                    freeList = block;
                }
            };

            std::vector<SizeClass> sizeClasses;

        public:
            MemoryPool(MemoryPoolInput input) {

                if (input.size_of_chunk.size() != input.count || input.number_of_chunks.size() != input.count) {
                    throw std::invalid_argument("size_of_chunk and number_of_chunks must have the same size as count");
                }

                // Rezerwujemy miejsce w wektorze sizeClasses
                sizeClasses.reserve(input.count);

                for (size_t i = 0; i < input.count; ++i) {
                    sizeClasses.emplace_back(input.size_of_chunk[i], input.number_of_chunks[i]);
                }
            }



            ~MemoryPool() {
                for (auto& sizeClass : sizeClasses) {
                    sizeClass.ClearMemory();  // Metoda, która czyści całą zaalokowaną pamięć w sizeClass
                }
            }
            void* Allocate(size_t size) {
                for (auto& sizeClass : sizeClasses) {
                    if (size <= sizeClass.chunkSize) {
                        return sizeClass.Allocate();
                    }
                }
                throw std::bad_alloc(); // Rozmiar przekracza największą klasę
            }

            template <typename T, typename... Args>
            T* AllocateObject(Args&&... args) {
                void* ptr = Allocate(sizeof(T));  // Alokuje odpowiednią ilość pamięci
                return new (ptr) T(std::forward<Args>(args)...);  // Używa placement new, aby zainicjować obiekt
            }


            template <typename T>
            void DeallocateObject(T*& object) {
                if (object != nullptr) {
                    object->~T();  // Wywołanie destruktora obiektu
                    Deallocate(object, sizeof(T));  // Zwolnienie pamięci
                    object = nullptr;
                }

            }


            void Deallocate(void* ptr, size_t size) {
                for (auto& sizeClass : sizeClasses) {
                    if (size <= sizeClass.chunkSize) {
                        sizeClass.Deallocate(ptr);
                        return;
                    }
                }
                assert(false && "Invalid deallocation size!");
            }
        };
        /* example to use
    class MyClass {
        int x;
        int y;
        int z;
        int w;
    public:
        MyClass(int val1, int val2) {
            x = val1;
            y = val1;
            z = val2;
            w = val2;
        }
        void print() {
            std::cout << "X: " << x << "Y: " << y << "Z: " << z << "W: " << w<<std::endl;
        }
    };

    int main() {
        MemoryPool allocator;


        MyClass* myObject = allocator.AllocateObject<MyClass>(1, 3);
    ;

        myObject->print();
        allocator.DeallocateObject(myObject);

        return 0;
    }
    */
    }

    namespace BlockMemoryPool {
        class MemoryPool {
        private:
            char* memory;
            size_t totalSize;


            std::list<std::pair<size_t, size_t>> freeBlocks;

        public:
            MemoryPool(size_t size) : totalSize(size) {
                memory = new char[size];
                freeBlocks.push_back({ 0, size });  // Początkowo cały blok jest wolny
            }

            ~MemoryPool() {
                delete[] memory;
            }


            // Alokowanie obiektów konkretnego typu
            template <typename T, typename... Args>
            T* AllocateObject(Args&&... args) {
                size_t size = sizeof(T);  // Potrzebujemy miejsca na jeden obiekt typu T
                void* ptr = Allocate(size);  // Alokacja miejsca w pamięci
                return new (ptr) T(std::forward<Args>(args)...);  // Użycie placement new do stworzenia obiektu
            }

            template <typename T>
            void DeallocateObject(T*& object) {
                if (object != nullptr) {
                    object->~T();  // Wywołanie destruktora obiektu
                    Deallocate(object, sizeof(T));  // Zwolnienie pamięci
                    object = nullptr;
                }
            }
        private:
            void MergeFreeBlocks() {
                freeBlocks.sort();
                for (auto it = freeBlocks.begin(); it != freeBlocks.end(); ++it) {
                    auto nextIt = std::next(it);
                    if (nextIt != freeBlocks.end() && it->first + it->second == nextIt->first) {
                        it->second += nextIt->second;
                        freeBlocks.erase(nextIt);
                    }
                }
            }

            void* Allocate(size_t size) {
                if (size == 0) {
                    throw std::invalid_argument("Cannot allocate 0 bytes");
                }
                for (auto it = freeBlocks.begin(); it != freeBlocks.end(); ++it) {
                    auto& block = *it;

                    size_t blockStart = block.first;
                    size_t blockEnd = block.first + block.second;

                    if (blockEnd - blockStart >= size) {
                        void* ptr = memory + blockStart;

                        // Jeśli blok jest dokładnie rozmiaru 'size', usuń go z listy
                        if (blockEnd - blockStart == size) {
                            freeBlocks.erase(it);
                        }
                        else {
                            // Zmniejsz blok i zachowaj resztę wolnej przestrzeni
                            block.first += size;
                            block.second -= size;
                        }

                        return ptr;
                    }
                }
                throw std::bad_alloc();  // Brak dostępnej pamięci
            }

            void Deallocate(void* ptr, size_t size) {
                if (!ptr || size == 0) {
                    return; // Nie rób nic dla pustych wskaźników
                }
                size_t ptrIndex = static_cast<char*>(ptr) - memory;

                freeBlocks.push_back({ ptrIndex, size });
                MergeFreeBlocks();
            }


        };


        /*
        class Monster {
        public:
            Monster(int health, int attack)
                : health(health), attack(attack) {
                std::cout << "Monster created with " << health << " health and " << attack << " attack.\n";
            }

            ~Monster() {
                std::cout << "Monster destroyed.\n";
            }

            void ShowStats() const {
                std::cout << "Health: " << health << ", Attack: " << attack << "\n";
            }

        private:
            int health;
            int attack;
        };

        int main() {
            MemoryPool pool(1000);  // Pula pamięci 1000 bajtów

            // Alokacja obiektów typu Monster
            Monster* monster1 = pool.AllocateObject<Monster>(100, 10);  // Monster z 100 HP i 10 ataku
            Monster* monster2 = pool.AllocateObject<Monster>(80, 12);   // Monster z 80 HP i 12 ataku

            monster1->ShowStats();
            monster2->ShowStats();

            // Zwolnienie pamięci
            pool.DeallocateObject(monster1);
            pool.DeallocateObject(monster2);

            return 0;
        }
        */
    }
}




