#include "riskCalc.h"
#include "util.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <unistd.h>
#include <cstring>

namespace riskCalc
{
    struct Cell
    {   
        static const size_t ID_LEN = 64;
        char m_id[ID_LEN];
        Cell() = default;
        Cell(std::string id) : m_id()
        {
            size_t strSize = id.size();
            id.copy(m_id, ID_LEN);
            size_t end = std::min(strSize, ID_LEN);
            m_id[end] = '\0';
        };
    };

    std::vector<Cell> corrMatrixAxis{};

    struct shm_header_t
    {
        static const size_t KEY_LEN = 64;
        char shm_key[KEY_LEN] = "Ay Caramba";
        unsigned int numCells = 0;
    };

    void Init(std::vector<std::string>& instrumentList)
    {
        std::cout << "Before mmap\n";
        std::string shName = "/ajuan_shm001";
        int oflags = O_RDWR | O_CREAT;
        int shmFD = shm_open(shName.c_str(), oflags, 0644 );
        off_t   length = 100 * 1024;
        ftruncate(shmFD, length);
        unsigned char *ptr = (unsigned char *) mmap(NULL, length, 
                                      PROT_READ|PROT_WRITE,
                                      MAP_SHARED, shmFD, 0);

        shm_header_t header;
        header.numCells = 2;
        std::memcpy(ptr, &header, sizeof(shm_header_t));
        
        unsigned char* arrStartAddr = ptr + sizeof(shm_header_t);
        Cell* cellArr = new(arrStartAddr) Cell[2];
        std::string msg = "Dios mio";
        std::memcpy(&cellArr[1].m_id, msg.c_str(), Cell::ID_LEN);
        msg = "Chicta lej";
        std::memcpy(&cellArr[2].m_id, msg.c_str(), Cell::ID_LEN);

        // placement new for matrix

        std::cout << "Enter number to quit program\n";
        int x;
        std::cin >> x;

         munmap(ptr, length);
         close(shmFD);                   /*   Close file   */
         shm_unlink(shName.c_str());
    }

    void Init_Consumer()
    {
        int oflags=O_RDWR;

        std::string shName = "/ajuan_shm001";
        int shmFD = shm_open(shName.c_str(), oflags, 0644 );

        off_t length = 100 * 1024;
        ftruncate(shmFD, length);
        unsigned char *ptr = (unsigned char *) mmap(NULL, length, 
                                      PROT_READ|PROT_WRITE,
                                      MAP_SHARED, shmFD, 0);

        shm_header_t header;
        std::memcpy(&header, ptr, sizeof(shm_header_t));
        std::cout << "Shm key: " << header.shm_key << "\n";

        unsigned char* arrStartAddr = ptr + sizeof(shm_header_t);
        // Cell* cellArr = new(arrStartAddr) Cell[2];
        Cell* cellArr = reinterpret_cast<Cell*>(arrStartAddr);
        std::cout << "Cell 1 msg: " << cellArr[1].m_id << "\n";
        std::cout << "Cell 2 msg: " << cellArr[2].m_id << "\n";

        munmap(ptr, length);
        close(shmFD);
    }
    void Cleanup()
    {

    }

    std::vector<std::string> GetInstrumentVec()
    {
        std::vector<std::string> instrumentIdList;

        std::for_each(std::begin(corrMatrixAxis),
                      std::end(corrMatrixAxis),
                      [&instrumentIdList](const auto& inst)
                      {
                          instrumentIdList.emplace_back(inst.m_id);
                      });

        return instrumentIdList;
    }
} // namespace riskCalc
