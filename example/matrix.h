#include <vector>
#include <cstddef>
template <typename T> struct Matrix
{
	double col, row;
	std::vector<T> data;
	Matrix<T>(){}
	Matrix<T>(double columns, double rows) :col(columns), row(rows){data.resize(col*row,0);}
	void resize(size_t s1,size_t s2)
	{

		col=s1;
		row=s2;
		data.resize(col*row,0);
	}
};

template <typename T> Matrix<T> fastMult(Matrix<T>*a,Matrix<T>*b)
{
	Matrix<T> *c= new Matrix<T>(a->col,b->row);

	int ic=0;

	int cdata=0;
	for (int nbl =0 ; nbl < c->row;nbl++)
	{
		for (int nbc = 0; nbc< c->col;)
		{
			for (auto it = nbl,itb=nbc; itb<b->data.size()&&it < a->data.size(); itb+=(b->row), it++)
			{
				cdata=a->data[it]*b->data[itb];
				c->data[ic]+= cdata;
			}	
			ic++;
			nbc++;
		}
	}
	return *c;
}
/*
void printf_matrix(Matrix<int> m)
{
	for(int i=0; i< m.data.size();i++){
		printf("%d " , m.data[i]);
		if((i+1)%(int)m.row==0)
			printf("\n");
	}

}*/

 
