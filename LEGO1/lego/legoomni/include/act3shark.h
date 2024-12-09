#ifndef ACT3SHARK_H
#define ACT3SHARK_H

#include "legoanimactor.h"

// VTABLE: LEGO1 0x100d7920 LegoPathActor
// VTABLE: LEGO1 0x100d79f0 LegoAnimActor
// SIZE 0x1a8
class Act3Shark : public LegoAnimActor {
public:
	Act3Shark();

	// FUNCTION: LEGO1 0x100430d0
	const char* ClassName() const override // vtable+0x0c
	{
		// STRING: LEGO1 0x100f03a0
		return "Act3Shark";
	}

	void ParseAction(char*) override;       // vtable+0x20
	void VTable0x70(float p_time) override; // vtable+0x70

	// LegoAnimActor vtable
	virtual MxResult FUN_10042ce0(void*); // vtable+0x10

	MxFloat GetUnknown0x2c() { return m_unk0x2c; }

	void SetUnknown0x2c(MxFloat p_unk0x2c) { m_unk0x2c = p_unk0x2c; }

	// SYNTHETIC: LEGO1 0x10043030
	// Act3Shark::`scalar deleting destructor'

private:
	list<void*> m_unk0x1c;     // 0x1c
	undefined4 m_unk0x28;      // 0x28
	MxFloat m_unk0x2c;         // 0x2c
	undefined m_unk0x30[0x0c]; // 0x30
	Mx3DPointFloat m_unk0x3c;  // 0x3c
};

// STUB: LEGO1 0x10042c90
// List<void *>::~List<void *>
// TODO: Update once type is known.
// STUB to resolve diff in scalar dtor and not create a new one.

#endif // ACT3SHARK_H
