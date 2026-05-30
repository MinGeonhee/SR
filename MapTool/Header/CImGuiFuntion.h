#pragma once
class CImGuiFuntion
{
	template<class T>
	static void		RenderSlider(const char* label, T* value, T min, T max);

	template<class T>
	static	void		Render_Input(const char* , T type);

};

template<class T>
inline void CImGuiFuntion::RenderSlider(const char*, T type)
{
    if constexpr (std::is_same<T, int>::value)
        return ImGui::SliderInt(label, value, min, max);
    else if constexpr (std::is_same<T, float>::value)
        return ImGui::SliderFloat(label, value, min, max);
    else if constexpr (std::is_same<T, double>::value)
        return ImGui::SliderScalar(label, ImGuiDataType_Double, value, &min, &max);
    else
        static_assert(!sizeof(T), "Unsupported type for slider");
    return false;
}

template<class T>
inline void CImGuiFuntion::RenderSlider(const char* label, T* value, T min, T max)
{
    if constexpr(std::is_sa))
}

template<class T>
inline void CImGuiFuntion::Render_Input(const char*, T type)
{
}
