#include "mxeventpresenter.h"

#include "decomp.h"
#include "mxautolock.h"
#include "mxdsaction.h"
#include "mxdssubscriber.h"
#include "mxeventmanager.h"
#include "mxmisc.h"
#include "mxvariabletable.h"

#include <assert.h>

DECOMP_SIZE_ASSERT(MxEventPresenter, 0x54);

// FUNCTION: LEGO1 0x100c2b70
MxEventPresenter::MxEventPresenter()
{
	Init();
}

// FUNCTION: LEGO1 0x100c2d40
MxEventPresenter::~MxEventPresenter()
{
	Destroy();
}

// FUNCTION: LEGO1 0x100c2da0
void MxEventPresenter::Init()
{
	m_data = NULL;
}

// FUNCTION: LEGO1 0x100c2db0
MxResult MxEventPresenter::AddToManager()
{
	MxResult ret = FAILURE;

	if (EventManager()) {
		ret = SUCCESS;
		EventManager()->RegisterPresenter(*this);
	}

	return ret;
}

// FUNCTION: LEGO1 0x100c2de0
void MxEventPresenter::Destroy()
{
	if (EventManager()) {
		EventManager()->UnregisterPresenter(*this);
	}

	ENTER(m_criticalSection);

	if (m_data) {
		delete[] m_data;
	}

	Init();

	m_criticalSection.Leave();
}

// FUNCTION: LEGO1 0x100c2e30
void MxEventPresenter::CopyData(MxStreamChunk* p_chunk)
{
	m_data = new MxU8[p_chunk->GetLength()];
	memcpy(m_data, p_chunk->GetData(), p_chunk->GetLength());
}

// FUNCTION: LEGO1 0x100c2e70
void MxEventPresenter::ReadyTickle()
{
	MxStreamChunk* chunk = NextChunk();

	if (chunk) {
		CopyData(chunk);
		m_subscriber->FreeDataChunk(chunk);
		ParseExtra();
		ProgressTickleState(e_starting);
	}
}

// FUNCTION: LEGO1 0x100c2eb0
void MxEventPresenter::StartingTickle()
{
	MxStreamChunk* chunk = CurrentChunk();

	if (chunk && m_action->GetElapsedTime() >= chunk->GetTime()) {
		ProgressTickleState(e_streaming);
	}
}

// FUNCTION: LEGO1 0x100c2ef0
// FUNCTION: BETA10 0x10152d26
MxResult MxEventPresenter::PutData()
{
	AUTOLOCK(m_criticalSection);

	if (IsEnabled()) {
		if (m_currentTickleState >= e_streaming &&
			(m_currentTickleState <= e_repeating || m_currentTickleState == e_done)) {
			if (m_currentChunk && m_currentChunk->GetLength()) {
				if (m_data[12] == 2) {
					const char* data = (const char*) m_currentChunk->GetData();
					MxVariableTable* variableTable = VariableTable();

					const char* name = data;
					assert(name);
					data += strlen(data) + 1;

					const char* value = data;
					assert(value);
					data += strlen(data) + 1;

					variableTable->SetVariable(name, value);
				}

				if (m_currentTickleState == e_streaming) {
					m_subscriber->FreeDataChunk(m_currentChunk);
				}
				m_currentChunk = NULL;
			}
		}
	}

	return SUCCESS;
}
