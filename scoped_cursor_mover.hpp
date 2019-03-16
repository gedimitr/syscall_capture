#ifndef SCOPED_CURSOR_MOVER_HPP_
#define SCOPED_CURSOR_MOVER_HPP_

#include <cstdint>

class ManagedBuffer;

class ScopedCursorMover
{
public:
    ScopedCursorMover(ManagedBuffer &managed_buffer);
    ScopedCursorMover(ManagedBuffer &managed_buffer, uint32_t new_pos);

    ~ScopedCursorMover();

    ScopedCursorMover(const ScopedCursorMover &) = delete;
    ScopedCursorMover &operator=(const ScopedCursorMover &) = delete;

    bool setCursor(uint32_t pos);

private:
    ManagedBuffer &m_managed_buffer;
    uint32_t m_original_pos;
};

#endif
