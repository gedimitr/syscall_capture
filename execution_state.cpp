#include "thread_id.hpp"
#include "execution_state.hpp"

ExecutionState::ExecutionState() :
    m_process_id(getProcessId()) { }

int32_t ExecutionState::getProcessId()
{
    return m_process_id;
}
