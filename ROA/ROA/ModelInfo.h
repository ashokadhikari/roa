#ifndef MODEL_INFO_H
#define MODEL_INFO_H

struct frames
{
	int start;

	int end;
};

struct ModelInfo
{
	char* model_name;

	char* texture_name;

	frames attack;

	frames move;

	ModelInfo() {}
};

struct ModelLaLa : public ModelInfo
{
	ModelLaLa() : ModelInfo() 
	{
		model_name = "new.md2";

		texture_name = "texture.BMP";

		attack.start = 47;

		attack.end = 53;

		move.start = 39;

		move.end = 43;
	}
};

#endif