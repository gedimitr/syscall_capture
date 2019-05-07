#include "file_writer.hpp"
#include "managed_buffers.hpp"

#include "file_write_permit.hpp"

FileWritePermit::FileWritePermit(FileWriter &file_writer) :
    m_file_writer(file_writer)
{
    m_file_writer.lock();
}

FileWritePermit::~FileWritePermit()
{
    m_file_writer.unlock();
}

ManagedBuffer &FileWritePermit::getManagedBuffer()
{
    return m_file_writer.getManagedBuffer();
}
