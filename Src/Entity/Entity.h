#pragma once


namespace Entity
{
	union Entity final
	{
		union Validation final
		{
			ECS_Utility::ValidEntityID m_validateID;
			struct
			{
				ECS_Utility::ValidEntityID m_generation : 31,
											m_isAlive : 1;
			};
		};
		ECS_Utility::EntityID m_uid;
		struct
		{
			ECS_Utility::EntityIndex m_index;
			Validation m_validation;
		};

		bool operator==(const Entity& _otherEnt) const noexcept
		{
			return m_uid == _otherEnt.m_uid;
		}

		bool IsAlive() const noexcept
		{
			return m_validation.m_isAlive;
		}

	};
	static_assert(sizeof(ECS_Utility::EntityID) == sizeof(Entity));

	struct EntityRecord final
	{
		//archetype needed
		Archetype::Archetype* m_archetype;
		int index;
		Entity::Validation validation;
	};
}

