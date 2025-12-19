#pragma once

class vec3;

class alignas(16) quat
{
	public:

	union
	{
		#ifdef USE_SIMD
		__m128	data;
		#endif
		struct
		{
			float	w;
			float	x;
			float	y;
			float	z;
		};
	};

	quat() noexcept;
	quat(const vec3& v3) noexcept;
	quat(float w, float x, float y, float z) noexcept;
	quat(float w, const vec3& v3) noexcept;
	quat(const quat& other) noexcept;
	quat&	operator=(const quat& other) noexcept;
	~quat() = default;

	quat	clone() const noexcept;
	quat& invert() noexcept;
	quat	inverted() const noexcept;

	quat&	normalize() noexcept;
	quat	normalized() const noexcept;
	quat&	fastNormalize() noexcept;
	quat	fastNormalized() const noexcept;

	static vec3 rotate(const vec3& rotateAround, quat rotation) noexcept;
	
	private:
	
	void	angleToVec() noexcept;

	static quat product(const quat& a, const quat& b) noexcept;
};