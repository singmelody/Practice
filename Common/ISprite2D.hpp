#pragma once

namespace Dream
{
	class IEngine;
	class Sprite;

	class ISprite2D
	{
	public:
		ISprite2D(){}
		virtual ~ISprite2D(){}

		virtual bool Init(IEngine* ) = 0;
		virtual void Destroy() = 0;

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		virtual Sprite* CreateSprite() = 0;

	};

	extern ISprite2D*	gSprite2D;
}

#define DEFINE_SPRITE2D_INTERFACE  Dream::ISprite2D*  Dream::gSprite2D = NULL;

#define CREATE_SPRITE2D_DLL_FUNC   "CreateSprite2D"
typedef void(*CreateSprite2DFunc)(Dream::ISprite2D**);