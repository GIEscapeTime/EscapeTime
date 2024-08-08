#pragma once

#include "../DXTK/Inc/SpriteBatch.h"
#include "../DXTK/Inc/CommonStates.h"
#include "DeviceAdapter.h"
#include <memory>

class SpriteAdapter
{
public:
	static SpriteAdapter& GetInstance() {
		static SpriteAdapter instance;
		return instance;
	}

	void Init() { m_states = std::make_unique<CommonStates>(DeviceAdapter::GetInstance().GetDevice().Get()); } ;
	void SetSpriteBatch(DirectX::SpriteBatch* batch) { if (m_batch == nullptr) m_batch = batch; };
	void SpriteBatchBegin() { m_batch->Begin(DX11::SpriteSortMode_Deferred, m_states->NonPremultiplied()); }
	DirectX::SpriteBatch* GetSpriteBatch() const { return m_batch; }

private:
	DirectX::SpriteBatch* m_batch;
	std::unique_ptr<DirectX::CommonStates> m_states;
};

