#ifndef FILE_WRITE_PERMIT_HPP
#define FILE_WRITE_PERMIT_HPP

class FileWriter;
class ManagedBuffer;

class FileWritePermit
{
public:
    FileWritePermit(FileWriter &file_writer);
    ~FileWritePermit();

    ManagedBuffer &getManagedBuffer();

private:
    FileWriter &m_file_writer;
};

#endif // FILE_WRITE_PERMIT_HPP
