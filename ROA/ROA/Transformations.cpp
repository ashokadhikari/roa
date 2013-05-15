#include "Transformations.h"

Vector3D PointToLocalSpace(Vector3D base_pos, Vector3D base_heading, Vector3D pos)
{
	Matrix Trans(4, 4);

	Matrix Rot(4, 4);

	Trans = Trans.TranslateMatrix(base_pos * (-1.0));

	Vector3D temp = pos;

	Vector3D v2(1.0, 0.0, 0.0);

	Vector3D v = base_heading;

	v.Normalize();

	double angle = acos(v.Dot(v2))*180.0/PI;

	if(v.z > 0)
			angle = (360.0 - angle);

	Rot = Rot.RotateMatrix(angle);

	Trans = Rot * Trans;

	Matrix position(4,1);

	position.elements[0][0] = pos.x;
	position.elements[1][0] = pos.y;
	position.elements[2][0] = pos.z;
	position.elements[3][0] = 1.0;

	position = Trans * position;

	pos.x = position.elements[0][0];
	pos.y = position.elements[1][0];
	pos.z = position.elements[2][0];

	/*ofstream outfile;
	outfile.open("dg sir.txt",ios::app);
	outfile << base_heading.x << "      " << base_heading.y << "       " << base_heading.z << endl;

	outfile << base_pos.x << "    "<< base_pos.y <<  "    "<<base_pos.z << endl;

	outfile << temp.x << "    "<< temp.y <<  "    "<<temp.z << endl;

	outfile << pos.x << "    "<< pos.y <<  "    "<<pos.z << endl << endl << endl;
	outfile.close();*/

	return pos;
}
