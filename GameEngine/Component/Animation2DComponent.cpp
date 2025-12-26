#include "Animation2DComponent.h"
#include "../Asset/Animation2D/Animation2D.h"
#include "../Asset/Animation2D/Animation2DManager.h"
#include "../Asset/AssetManager.h"
#include "MeshComponent.h"

CAnimation2DComponent::CAnimation2DComponent()
{
}

CAnimation2DComponent::CAnimation2DComponent(
	const CAnimation2DComponent& ref)	:
	CObjectComponent(ref)
{
	mUpdateComponent = ref.mUpdateComponent;
}

CAnimation2DComponent::CAnimation2DComponent(
	CAnimation2DComponent&& ref) noexcept :
	CObjectComponent(std::move(ref))
{
}

CAnimation2DComponent::~CAnimation2DComponent()
{
}

void CAnimation2DComponent::SetUpdateComponent(
	const std::weak_ptr<class CMeshComponent>& Component)
{
	mUpdateComponent = Component;

	auto	MeshComponent = mUpdateComponent.lock();

	if (MeshComponent && mCurrentAnimation)
	{
		auto	Asset = mCurrentAnimation->GetAsset().lock();

		if (Asset)
		{
			if (MeshComponent->SetTexture(0, 0,
				Asset->GetTexture()))
				mUpdateEnable = true;
		}
	}
}

void CAnimation2DComponent::AddAnimation(
	const std::weak_ptr<class CAnimation2D>& Anim, float PlayTime,
	float PlayRate, bool Loop, bool Reverse)
{
	auto	_Anim = Anim.lock();

	if (!_Anim)
		return;

	auto	iter = mAnimationMap.find(_Anim->GetName());

	if (iter != mAnimationMap.end())
		return;

	std::shared_ptr<CAnimation2DSequence>	Sequence(new CAnimation2DSequence);

	Sequence->SetAnimation2D(Anim);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(Loop);
	Sequence->SetReverse(Reverse);

	mAnimationMap.insert(std::make_pair(_Anim->GetName(), Sequence));

	if (!mCurrentAnimation)
	{
		mCurrentAnimation = Sequence;

		auto	MeshComponent = mUpdateComponent.lock();

		if (MeshComponent && mCurrentAnimation)
		{
			auto	Asset = mCurrentAnimation->GetAsset().lock();

			if (Asset)
			{
				if (MeshComponent->SetTexture(0, 0, Asset->GetTexture()))
					mUpdateEnable = true;
			}
		}
	}
}

void CAnimation2DComponent::AddAnimation(const std::string& Name, 
	float PlayTime, float PlayRate, bool Loop, bool Reverse)
{
	auto	AnimMgr = CAssetManager::GetInst()->GetAnimation2DManager().lock();

	auto	Anim = AnimMgr->FindAnimation(Name);

	if (Anim.expired())
		return;

	auto	_Anim = Anim.lock();

	auto	iter = mAnimationMap.find(_Anim->GetName());

	if (iter != mAnimationMap.end())
		return;

	std::shared_ptr<CAnimation2DSequence>	Sequence(new CAnimation2DSequence);

	Sequence->SetAnimation2D(Anim);
	Sequence->SetPlayTime(PlayTime);
	Sequence->SetPlayRate(PlayRate);
	Sequence->SetLoop(Loop);
	Sequence->SetReverse(Reverse);

	mAnimationMap.insert(std::make_pair(_Anim->GetName(), Sequence));
	
	if (!mCurrentAnimation)
	{
		mCurrentAnimation = Sequence;

		auto	MeshComponent = mUpdateComponent.lock();

		if (MeshComponent && mCurrentAnimation)
		{
			auto	Asset = mCurrentAnimation->GetAsset().lock();

			if (Asset)
			{
				if (MeshComponent->SetTexture(0, 0,
					Asset->GetTexture()))
					mUpdateEnable = true;
			}
		}
	}
}

void CAnimation2DComponent::SetPlayTime(const std::string& Name,
	float PlayTime)
{
	auto	iter = mAnimationMap.find(Name);

	if (iter == mAnimationMap.end())
		return;

	iter->second->SetPlayTime(PlayTime);
}

void CAnimation2DComponent::SetPlayRate(const std::string& Name,
	float PlayRate)
{
	auto	iter = mAnimationMap.find(Name);

	if (iter == mAnimationMap.end())
		return;

	iter->second->SetPlayRate(PlayRate);
}

void CAnimation2DComponent::SetLoop(const std::string& Name,
	bool Loop)
{
	auto	iter = mAnimationMap.find(Name);

	if (iter == mAnimationMap.end())
		return;

	iter->second->SetLoop(Loop);
}

void CAnimation2DComponent::SetReverse(const std::string& Name,
	bool Reverse)
{
	auto	iter = mAnimationMap.find(Name);

	if (iter == mAnimationMap.end())
		return;

	iter->second->SetReverse(Reverse);
}

void CAnimation2DComponent::ChangeAnimation(const std::string& Name)
{
	if (mUpdateComponent.expired())
		return;

	else if (!mCurrentAnimation)
		return;

	else if (mCurrentAnimation->GetName() == Name)
		return;

	auto	iter = mAnimationMap.find(Name);

	if (iter == mAnimationMap.end())
		return;

	// 애니메이션 정보를 초기화한다.
	mCurrentAnimation->Clear();

	// 현재 애니메이션을 교체한다.
	mCurrentAnimation = iter->second;

	mCurrentAnimation->Clear();

	// 등록된 MeshComponent가 가지고 있는 Material의 Texture를
	// 애니메이션이 가지고 있는 Texture로 교체한다.
	auto	MeshComponent = mUpdateComponent.lock();

	if (MeshComponent)
	{
		auto	Asset = mCurrentAnimation->GetAsset().lock();

		if (Asset)
			MeshComponent->SetTexture(0, 0, Asset->GetTexture());

		if (mCurrentAnimation->GetReverse())
		{
			MeshComponent->SetAnimationFrame(
				mCurrentAnimation->GetFrameCount() - 1);
		}

		else
			MeshComponent->SetAnimationFrame(0);
	}
}

bool CAnimation2DComponent::Init()
{
	return true;
}

void CAnimation2DComponent::Update(float DeltaTime)
{
	if (mCurrentAnimation)
	{
		auto	MeshComponent = mUpdateComponent.lock();

		if (!mUpdateEnable)
		{
			if (MeshComponent)
			{
				auto	Asset = mCurrentAnimation->GetAsset().lock();

				if (Asset)
				{
					if (MeshComponent->SetTexture(0, 0,
						Asset->GetTexture()))
					{
						mUpdateEnable = true;
						MeshComponent->SetAnimationEnable(true);
					}
				}
			}
		}

		mCurrentAnimation->Update(DeltaTime);

		int	Frame = mCurrentAnimation->GetFrame();

		if (MeshComponent)
		{
			MeshComponent->SetAnimTextureType(mCurrentAnimation->GetAnimationTextureType());
			MeshComponent->SetAnimationFrame(Frame);
		}
	}
}

void CAnimation2DComponent::PostUpdate(float DeltaTime)
{
}

void CAnimation2DComponent::Destroy()
{
	CObjectComponent::Destroy();
}

CAnimation2DComponent* CAnimation2DComponent::Clone()	const
{
	return new CAnimation2DComponent(*this);
}
