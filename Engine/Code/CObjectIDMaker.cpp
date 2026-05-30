#include "CObjectIDMaker.h"

uint32_t CObjectIDMaker::Make_ObjectID(ObjectHandle handle)
{
	uint32_t	_ObjectID;

	_ObjectID = (static_cast<uint32_t>(handle.m_eType) << 24) | static_cast<uint32_t>(handle.m_iIndex);
	
	return _ObjectID;
}