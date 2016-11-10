#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <stddef.h>
# include <map>
# include <queue>
using namespace cv;
using namespace std;
Mat final_image;
int **image;
//int **image_copy;
//int **image_dct;
int r1, c1;
int ** image_matrix;
ofstream fout("encoded_file.txt");
ofstream f("Encoded_codes.txt");
ofstream fo("Run_length.txt");
long long int numb_bytes = 0;
int **Decode;
double **image_after_idct;
map <int, int> m;
map <string,int> codes1;
map <int, string> codes2;
int r, c;
int **image_dct;
int **image_idct;
int maxi = 0;
int numb = 1;
//int image_matrix1[8][8]= { { 52,55,61,66,70,61,64,73 },{ 63,59,55,90,109,85,69,72 },{ 62,59,68,113,144,104,66,73 },{ 63,58,71,122,154,106,70,69 },{ 67,61,68,104,126,88,68,70 },{ 79,65,60,70,77,68,58,75 },{ 85,71,64,59,55,61,65,83 },{ 87,79,69,68,65,76,78,94 } };;
struct Node
{
	struct Node *left_child;
	struct Node *right_child;
	int value;
	int frequency;
	Node(int data, int freq)
	{
		left_child = right_child = NULL;
		this->value = data;
		this->frequency = freq;
	}
};

struct minimum_func
{
	bool operator()(Node* l, Node* r)
	{
		return (l->frequency > r->frequency);
	}
};

void Traverse(struct Node* root, string str)
{
	if (root == NULL)
		return;

	if (root->value != -450) 
	{
		f << root->value << " | " << str << "\n";

		/*if (root->value == 36)
			cout << "Rock" << endl;*/
		codes1[str] = root->value;
		codes2[root->value] = str;
	}
	Traverse(root->left_child, str + "0");
	Traverse(root->right_child, str + "1");
}
void Traverse1(struct Node* root)
{
	if (root == NULL)
		return;

	/*if (root->value != '-450')
	{
		f << root->value << " | " << str << "\n";

		/*if (root->value == 36)
		cout << "Rock" << endl;*/
		/*codes1[str] = root->value;
		codes2[root->value] = str;
	}*/
	cout << root->value << " " << root->frequency << endl;
	Traverse1(root->left_child);
	Traverse1(root->right_child);
}
void create_tree()
{
	map <int, int> ::iterator it;
	struct Node *Node1, *Node2, *Node3;
	Node1 = new Node(0, 0);
	Node2 = new Node(0, 0);
	Node3 = new Node(0, 0);
	priority_queue<Node*, vector<Node*>, minimum_func> Heap;
	for (it = m.begin(); it != m.end(); it++)
	{
		//cout << it->first << " " << it->second << endl;
		//if (it->first == 36)
			//cout << "Himanshu Tolani" << endl;
		Heap.push(new Node(it->first, it->second));
	}

	

	while (Heap.size() != 1)
	{
		// Extract the two minimum freq items from min heap
		Node1 = Heap.top();
		Heap.pop();

		Node2 = Heap.top();
		Heap.pop();

		// Create a new internal node with frequency equal to the
		// sum of the two nodes frequencies. Make the two extracted
		// node as left and right children of this new node. Add
		// this node to the min heap
		// '$' is a special value for internal nodes, not used
		Node3 = new Node(-450, Node1->frequency + Node1->frequency);
		Node3->left_child = Node1;
		Node3->right_child = Node2;
		Heap.push(Node3);
	}

	// Print Huffman codes using the Huffman tree built above
	//Traverse1(Heap.top());
	Traverse(Heap.top(), "");
}


void Run_length()
{
	//cout << "welcome";
	
	//cout << "PLLL" << endl;
	int current = 0;
	//cout << "Hello" << endl;
	int length = 0;
	//cout << "PK" << endl;
	int count = 0;
	int line = 0;
	//cout << "Hello" << endl;
	for (int i = 0; i < r1; i++)
	{
		for (int j = 0; j < c1; j++)
		{
			//count++;
			/*if (i == 48 && j == 313)
			{
				cout << image[i][j] << endl;
			}*/
			if (i == 0 && j == 0)
			{
				current = image_dct[i][j];
				length++;
			}
			
			else
			{
				if (current != image_dct[i][j])
				{
					/*if (i == 48 && j == 313)
					cout << line << endl;*/
					fo << current << " " << length << endl;
					//line++;
					m[current] = m[current]+length;
					m[length]++;
					numb++;
					if (abs(current) > maxi)
						maxi = abs(current);

					if (abs(length) > maxi)
						maxi = abs(length);
					count = count + length;
					current = image_dct[i][j];
					length = 1;
					//if(i==48 && j==313)
					//cout << "Hello" << endl;
				}
				else
				{
					length++;
					/*if (i == 48 && j == 312)
					cout << "Hii" << endl;*/
				}
			}

		}
	}
	fo << current << " " << length <<endl;
	m[current] = m[current] + length;
	m[length]++;
	count = count + length;

	//cout << count << endl;
}
void pad(Mat img)
{
	r = img.rows;
	c = img.cols;
	cout << r << " " << c << endl;

	 r1 = r+8-r%8;
	 c1 = c+8-c%8;
	cout << r1 << " "<<c1 << endl;

	
	image_matrix = new int*[r1];
	for (int i = 0; i<r1; i++)
		image_matrix[i] = new int[c1];
	
	image = new int*[r1];
	for (int i = 0; i<r1; i++)
		image[i] = new int[c1];

	Mat final_image(r1, c1, CV_8UC1, Scalar(0));

	for (int i = 0; i <r1; i++)
	{
		for (int j = 0; j <c1; j++)
		{
			if (i >= r || j >= c)
				image_matrix[i][j] = -128;
			else
			image_matrix[i][j] = (int)(img.at<uchar>(i, j))-128;
			
			//cout << " |" << i << ":" << j << ":"<< image_matrix[i][j]<<"| ";
			image[i][j] = image_matrix[i][j];
		}
		//cout << endl;
		//cout << i << " " << j << endl;

   }

	//cout << image_matrix[445][555] << endl;

	for (int i = 0; i <r1; i++)
	{
		for (int j = 0; j <c1; j++)
		{
			//final_image[i][j] = image_matrix[i][j];
			(final_image.at<uchar>(i, j)) = (uchar)image_matrix[i][j];
		}
		//cout << endl;
	}

	image_dct = new int*[r1];
	for (int i = 0; i<r1; i++)
		image_dct[i] = new int[c1];

	image_idct = new int*[r1];
	for (int i = 0; i<r1; i++)
		image_idct[i] = new int[c1];
	//cout << final_image.rows << " himansh " << final_image.cols << endl;
	namedWindow("Image", WINDOW_NORMAL);
	imshow("Image", final_image);
	waitKey(0);
}


void DCT(int x1,int y1)
{
	//int ** image_after_dct;
	//cout << "HIMANSHU" << endl;
	
	
	double Y[8][8];
	int Z[8][8] = { {16,11,10,16,24,40,51,61},{12,12,14,19,26,58,60,55},{14,13,16,24,40,57,69,56},{14,17,22,29,51,87,80,62},{18,22,37,56,68,109,103,77},{24,35,55,64,81,104,113,92},{49,64,78,87,103,121,120,101},{72,92,95,98,112,100,103,99} };

	//cout << "HT" << endl;
	for (int u = 0; u < 8; u++)
	{
		///cout << "Him " << u<<endl;
		for (int v = 0; v < 8; v++)
		{
			//cout << "Tol " << v << endl;
			double c1 = 0;
			double c2 = 0;
			double sum = 0;
			if (u == 0)
				c1 = 0.707;
			else
				c1 = 1;


			if (v == 0)
				c2 = 0.707;
			else
				c2 = 1;
			for (int m = 0; m < 8; m++)
			{
				for (int n = 0; n < 8; n++)
				{
					double theta1 = ((2 * m + 1)*u*3.14) / 16.0;
					double theta2 = ((2 * n + 1)*v*3.14) / 16.0;
					sum = sum + image[m+x1][n+y1] * cos(theta1)*cos(theta2);

				}
			}
			//cout << "KKP" << endl;
			Y[u][v] = ((2*c1*c2) *sum)/8.0;
			//image[u + x1][v + y1] = Y[u][v];
			//cout << "Tol" << endl;
			//cout << (u + x1) << " " << v + y1 << endl;
			//image[u + x1][v + y1] = Y[u][v];
			image_dct[u + x1][v + y1] = Y[u][v];
			//cout << "Perfect" << endl;
		

		}
		
	}

	//cout << "KP";
	
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			//image_matrix[i + x1][j + y1] = image_matrix[i + x1][j + y1] / Z[i][j];
			//image[i + x1][j + y1] = image[i + x1][j + y1]/Z[i][j];
			image_dct[i + x1][j + y1] = image_dct[i + x1][j + y1]/Z[i][j];
			//cout << image[i+x1][j+y1] << " ";
		}
		//cout << endl;
	}

}
void create_encoded_file()
{
	ifstream file("Run_length.txt");
	int a, b;
	int line = 0;
	int count_check = 0;
	int check = 0;
	while (file >> a >> b)
	{
		fout << codes2[a] << " " << codes2[b] << endl;
		/*if (line == 9316)
		{
			cout << a << " " << b << endl;
			cout << codes2[a] << " Him "<<codes2[b] << endl;
		}
			//cout << a << " him " << b << endl;
		line++;*/
			//check++;
		

		numb_bytes += codes2[a].length() + codes2[b].length();

	}

}
void IDCT(int x1, int y1)
{
	double Y[8][8];
	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			double c1 = 0;
			double c2 = 0;
			double sum = 0;
			
			for (int u = 0; u < 8; u++)
			{
				for (int v = 0; v < 8; v++)
				{
					if (u == 0)
						c1 = 0.707;
					else
						c1 = 1;


					if (v == 0)
						c2 = 0.707;
					else
						c2 = 1;
					double theta1 = ((2 * x + 1)*u*3.14) / 16.0;
					double theta2 = ((2 * y + 1)*v*3.14) / 16.0;
					sum = sum + c1*c2*Decode[u + x1][v + y1] * cos(theta1)*cos(theta2);

				}
			}

			Y[x][y] = (sum) / 4.0;
			image_idct[x + x1][y + y1] = Y[x][y]+128;


		}

	}

	//cout << x1 << " " << y1 << endl;

/*	for (int i = 0; i < 8; i++)
	{
		for ( int j = 0; j < 8; j++)
		{
			cout << Y[i][j] << " ";
		}
		cout << endl;
	}
*/

}
void Dequantize(int x1,int y1)
{
	int Z[8][8] = { { 16,11,10,16,24,40,51,61 },{ 12,12,14,19,26,58,60,55 },{ 14,13,16,24,40,57,69,56 },{ 14,17,22,29,51,87,80,62 },{ 18,22,37,56,68,109,103,77 },{ 24,35,55,64,81,104,113,92 },{ 49,64,78,87,103,121,120,101 },{ 72,92,95,98,112,100,103,99 } };
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Decode[i + x1][j + y1] = Decode[i + x1][j + y1] * Z[i][j];
			//cout << image[i+x1][j+y1] << " ";
		}
		//cout << endl;
	}

	IDCT(x1, y1);


}

void Unpad(Mat Img)
{
	int **image_matrix_copy;
	image_matrix_copy = new int*[r];
	for (int i = 0; i<r; i++)
		image_matrix_copy[i] = new int[c];


	Mat final_image(r, c, CV_8UC1, Scalar(0));

	for (int i = 0; i <r; i++)
	{
		for (int j = 0; j <c; j++)
		{
			
				image_matrix_copy[i][j] = (int)(Img.at<uchar>(i, j));
		}
	

	}


	for (int i = 0; i <r; i++)
	{
		for (int j = 0; j <c; j++)
		{
			//final_image[i][j] = image_matrix[i][j];
			(final_image.at<uchar>(i, j)) = (uchar)image_matrix_copy[i][j];
		}
		//cout << endl;
	}


	
	namedWindow("Image", WINDOW_NORMAL);
	imshow("Image", final_image);
	waitKey(0);
}
void Image_decode()
{
	
	//cout << image[48][312] << " Himanshu" << endl;
	//cout << "HT" << endl;
	ifstream in("encoded_file.txt");
	//cout << "KP" << endl;
	Decode = new int*[r1];
	for (int i = 0; i<r1; i++)
		Decode[i] = new int[c1];
	string a, b;
	int c = 0;
	int i=0, j=0;
	//cout << "DP" << endl;
	while (in >> a >> b)
	{
		//cout << "GUP" << endl;
		//cout << a << " " <<b<< endl;
		int l1 = codes1[a];
		int l2 = codes1[b];
		//cout << l1 << " " << l2 << endl;
		int k =l2;
		for (int m=1; m <= k; m++)
		{
			Decode[i][j] = l1;
			j++;
			if (j == c1)
			{
				j = 0;
				i++;
			}

			
		}
		//cout << "Himanshu" << endl;
	}

	image_after_idct = new double*[r1];
	for (int i = 0; i<r1; i++)
		image_after_idct[i] = new double[c1];
	cout << "*****DeQuantizing Image *********" << endl;
	for (int i = 1; i <= r1 / 8; i++)
	{
		for (int j = 1; j <= c1 / 8; j++)
		{
			Dequantize((i - 1) * 8, (j - 1) * 8);
			//cout << (i - 1) * 8 << " " << (j - 1) * 8 << endl;
		}
	}

	/*ofstream fout1("check.txt");
	for ( i = 0; i < r1; i++)
	{
		for (j = 0; j < c1; j++)
		{
			
				
			fout1 << Decode[i][j] << " ";

			
		}
		cout << endl;
	}*/
	cout << "******** Performing IDCT *********" << endl;
	//Mat Image_aft = final_image.clone();
	Mat Image_aft(r1, c1, CV_8UC1, Scalar(0));
	for (int i = 0; i <r1; i++)
	{
		for (int j = 0; j <c1; j++)
		{
			//final_image[i][j] = image_matrix[i][j];
			//cout << Decode[i][j] << " " << image[i][j] << endl;
			Decode[i][j] = image_idct[i][j];
			(Image_aft.at<uchar>(i, j)) = (uchar)Decode[i][j];
		}
		//cout << endl;
	}
	namedWindow("Final_Image", WINDOW_AUTOSIZE);
	imshow("Final_Image", Image_aft);
	waitKey(0);
	

	Unpad(Image_aft);

}

int calc(int num)
{
	int c = 0;
	while (num > 0)
	{
		num = num / 2;
		c++;
	}
	return num;
}

int main()
{


	
	string s1 = "test_image.tif";
	//string s2 = "image2.jpg";
	Mat image;
	
	image = imread(s1, CV_LOAD_IMAGE_GRAYSCALE);
	r = image.rows;
	c = image.cols;
	cout << "******* Encoding the Image **********" << endl;
	cout << "**** PADDING THE IMAGE TO MULTIPLE OF 8 *******" << endl;
	pad(image);


	//_---------------------------------------------------------------------------------------//
	int ** image_matrix1;
	image_matrix1 = new int*[r1];
	for (int i = 0; i<image.rows; i++)
		image_matrix1[i] = new int[c1];
	 

	
	//image_matrix1[0] = &Array[0];
	
	
	
	
	
	
	/*Decode = new int*[image.rows];
	for (int i = 0; i<image.rows; i++)
		Decode[i] = new int[image.cols];*/
	for (int i = 0; i <image.rows; i++)
	{
		for (int j = 0; j <image.cols; j++)
		{
			image_matrix1[i][j] = (int)(image.at<uchar>(i, j));
			//Decode[i][j] = image_matrix1[i][j];
			//cout << image_matrix1[i][j] << " " << Decode[i][j] << endl;
		}
		//cout << endl;
	}
	
	//cout << r1 << " " << c1 << " " << r << " " << c;
	   
	cout << "***** calculating the DCT for subimages ******" << endl;
	for (int i = 1; i <= r1 / 8; i++)
	{
		//cout << "Care" << endl;
		for (int j = 1; j <= c1 / 8; j++)
		{
			//cout << "free" << endl;
			DCT((i-1)*8, (j-1)*8);
			//cout << (i - 1) * 8 << " " << (j - 1) * 8 << endl;
		}
	}
	//cout << "HT";
	cout << r1 << " " << c1 << endl;
	cout << "***** calculating the run length Coding *********" << endl;
	Run_length();
	cout << "********* creating the Huffmann Tree ********" << endl;
	create_tree();
	cout << "****** creating the encoding file ********" << endl;
	create_encoded_file();
	cout << "Number of Bits used in actual Image- " << r*c * 8 << endl;
	cout << "Total Number of Bits used - ";
	cout << numb_bytes << endl;
	cout << "Ratio of Bits used  -";
	cout << (double)numb_bytes / (r * c * 8) << endl;


	cout << "************ DECODING THE IMAGE **********" << endl;

	Image_decode();
	
	int num = calc(maxi);
	//cout << codes2[36] << endl;
	//cout << "Amount of bits required if applying only run length coding are- ";
	//cout << (num + 1)*numb * 2 << endl;
	//cout << "compression ratio - " << (double)(num + 1)*numb * 2*2 / (r*c*8.0) << endl;
	namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", image);
	waitKey(0);


	// Wait for a keystroke in the window


	//int A[8][8]

	


	return 0;



}