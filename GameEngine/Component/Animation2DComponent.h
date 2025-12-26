#pragma once

#include "ObjectComponent.h"
#include "Animation2DSequence.h"

class CAnimation2DComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CAnimation2DComponent();
	CAnimation2DComponent(const CAnimation2DComponent& ref);
	CAnimation2DComponent(CAnimation2DComponent&& ref)	noexcept;

public:
	virtual ~CAnimation2DComponent();

protected:
	std::weak_ptr<class CMeshComponent>	mUpdateComponent;
	std::unordered_map<std::string, std::shared_ptr<CAnimation2DSequence>>	mAnimationMap;

	std::shared_ptr<CAnimation2DSequence>	mCurrentAnimation;

	bool	mUpdateEnable = false;

public:
	void SetUpdateComponent(
		const std::weak_ptr<class CMeshComponent>& Component);
	void AddAnimation(const std::weak_ptr<class CAnimation2D>& Anim,
		float PlayTime = 1.f, float PlayRate = 1.f, 
		bool Loop = false, bool Reverse = false);
	void AddAnimation(const std::string& Name,
		float PlayTime = 1.f, float PlayRate = 1.f,
		bool Loop = false, bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayRate(const std::string& Name, float PlayRate);
	void SetLoop(const std::string& Name, bool Loop);
	void SetReverse(const std::string& Name, bool Reverse);
	void ChangeAnimation(const std::string& Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Destroy();

protected:
	virtual CAnimation2DComponent* Clone()	const;
};

