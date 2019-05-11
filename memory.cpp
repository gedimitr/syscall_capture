#include "syscall.h"
#include <sys/mman.h>
#include <unistd.h>

#include "libsyscall_intercept_hook_point.h"

#include "memory.hpp"

namespace {

uint32_t getPagesize()
{
    static long page_size = sysconf(_SC_PAGESIZE);
    return static_cast<uint32_t>(page_size);
}

uint32_t convertToMultiplePageSize(uint32_t size)
{
    uint32_t page_size = getPagesize();
    if (size % page_size) {
        uint32_t num_pages = (size / page_size) + 1;
        return page_size * num_pages;
    } else {
        return size;
    }
}

}

void *mapMemory(unsigned long length)
{
    long res = syscall_no_intercept(SYS_mmap, 0, length, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    void *addr = reinterpret_cast<void *>(res);

    return (addr != MAP_FAILED) ? addr : nullptr;
}

bool unmapMemory(void *addr, unsigned long length)
{
    long res = syscall_no_intercept(SYS_munmap, addr, length);
    return (res == 0);
}

MappedMemory::MappedMemory(uint32_t size)
{
    m_length = convertToMultiplePageSize(size);
    m_addr = mapMemory(m_length);
}

MappedMemory::~MappedMemory()
{
    if (!isEmpty()) {
        free();
    }
}

uint32_t MappedMemory::getSize() const
{
    return m_length;
}

void *MappedMemory::data()
{
    return m_addr;
}

const void *MappedMemory::data() const
{
    return m_addr;
}

bool MappedMemory::allocate(uint32_t size)
{
    if (!isEmpty()) {
        free();
    }

    m_length = convertToMultiplePageSize(size);
    m_addr = mapMemory(m_length);

    return (m_addr != nullptr);
}

void MappedMemory::free()
{
    unmapMemory(m_addr, m_length);
}

bool MappedMemory::isEmpty() const
{
    return (m_addr == nullptr);
}
