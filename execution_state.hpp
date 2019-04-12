#ifndef EXECUTION_STATE_HPP
#define EXECUTION_STATE_HPP

#include <cstdint>

class ExecutionState
{
public:
    ExecutionState();

    int32_t getProcessId();

public:
    int32_t m_process_id;
};

#endif // EXECUTION_STATE_HPP
