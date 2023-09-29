#include "CoolBro.h"

using namespace Xeph2D;

void CoolBro::Serializables()
{
	SERIALIZE_DEFAULT;
}

void CoolBro::Start()
{
	Debug::LogColor(Color::Aqua);
	Debug::Log("Yo This is cool bro!");
}

void CoolBro::Update()
{
	
}
