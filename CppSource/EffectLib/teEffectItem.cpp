#include	"EffectLib\\Item.h"

Vector4 COLOR::Value_0from1()
{
	Vector4 out;
	out.x = (float)red / 255.0f;
	out.y = (float)green / 255.0f;
	out.z = (float)blue / 255.0f;
	out.w = (float)
		//255.0f;
		alpha / 255.0f;
	
	return out;
}