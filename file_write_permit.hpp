#ifndef FILE_WRITE_PERMIT_HPP
#define FILE_WRITE_PERMIT_HPP

class FileWriter;
class BufferView;

class FileWritePermit
{
public:
    FileWritePermit(FileWriter &file_writer);
    ~FileWritePermit();

    BufferView &getBufferView();

private:
    FileWriter &m_file_writer;
};

#endif // FILE_WRITE_PERMIT_HPP
