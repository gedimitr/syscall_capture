#include "file_writer.hpp"
#include "buffer_view.hpp"

#include "file_write_permit.hpp"

FileWritePermit::FileWritePermit(FileWriter &file_writer) :
    m_file_writer{file_writer}
{
    m_file_writer.lock();
}

FileWritePermit::~FileWritePermit()
{
    m_file_writer.unlock();
}

BufferView &FileWritePermit::getBufferView()
{
    return m_file_writer.getBufferView();
}
