#ifndef HEADER_WRITER_HPP_
#define HEADER_WRITER_HPP_

class ManagedBuffer;

class HeaderWriter
{
public:
    HeaderWriter(ManagedBuffer &managed_buffer);

    void write();

private:
    ManagedBuffer &m_managed_buffer;
};

#endif
