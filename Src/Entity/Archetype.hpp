namespace Archetype
{
	template<Tools::is_void_Fn CallbackType>
	void Archetype::CreateEntity(CallbackType&& _func) noexcept
	{
		_func();
	}
}