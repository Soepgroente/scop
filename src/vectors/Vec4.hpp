#pragma once

class vec4
{
	public:

	vec4();
	vec4(float x, float y, float z, float w);
	~vec4() = default;
	vec4(const vec4& other);

	vec4&	operator=(const vec4& other);

	union
	{
		struct
		{
			float	x;
			float	y;
			float	z;
			float	w;
		};
		float	elements[4];
	};
};