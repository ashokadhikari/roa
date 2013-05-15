#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <fstream>
#include "Vector3D.h"

using namespace std;

struct Matrix
{
	int row;
	int column;
	double **elements;

	Matrix()
	{
		row = column = 0;

		elements = NULL;
	}

	Matrix(int r, int c)
	{
		row = r;
		column = c;

		elements = new double*[row];

		for(int i = 0; i < row; i++)
		{
			elements[i] = new double[column];
		}

		for(int i = 0; i < row; i++)
		{
			for(int j = 0; j < column; j++)
			{
				if(i == j) elements[i][j] = 1;
				else elements[i][j] = 0;
			}
		}
	}

	void SetMatrix(double **el, int r, int c)
	{
		for(int i = 0; i < r; i++)
		{
			for(int j = 0; j < c; j++)
			{
				elements[i][j] = el[i][j];
			}
		}
	}

	Matrix operator* (Matrix mx)
	{
		if(column != mx.row) return(Matrix());

		Matrix temp(row, mx.column);

		for(int i = 0; i < temp.row; i++)
		{
			for(int j = 0; j < temp.column; j++)
			{
				temp.elements[i][j] = 0;
				for(int k = 0; k < row; k++)
				{
					temp.elements[i][j] += ( elements[i][k] * mx.elements[k][j] );
				}
			}
		}

		return temp;
	}

	Matrix TranslateMatrix(Vector3D v)
	{
		Matrix temp(4, 4);
	
		temp.elements[0][3] = v.x;
		temp.elements[1][3] = v.y;
		temp.elements[2][3] = v.z;

		return temp;
	}

	Matrix RotateMatrix(double angle)
	{
		Matrix temp(4, 4);

		angle = angle * PI / 180;
		temp.elements[0][0] = cos(angle);
		temp.elements[0][2] = -1.0 * sin(angle);

		temp.elements[2][0] = 1.0 * sin(angle);
		temp.elements[2][2] = 1.0 * cos(angle);

		return temp;
	}

	Matrix ScaleMatrix(Vector3D scale)
	{
		Matrix temp(4, 4);

		temp.elements[0][0] = scale.x;
		
		temp.elements[2][2] = scale.z;

		return temp;
	}

	void Show()
	{
		ofstream outfile;
		outfile.open("mk.txt",ios::app);
	


		for(int i = 0; i < row; i++)
		{
			for(int j = 0; j < column; j++)
			{
				outfile << elements[i][j] << "  ";
			}

			outfile << std::endl;
		}

		outfile << std::endl << std::endl << std::endl;
		outfile.close();
	}
	
};
#endif