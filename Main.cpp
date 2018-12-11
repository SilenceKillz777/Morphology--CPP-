#include <iostream>
#include <fstream>
using namespace std;

class morphology{
	public:
		
	int numRowsImg, numColsImg, minImg, maxImg;
	int numRowsStructElem, numColsStructElem, minStructElem, maxStructElem;
	int rowOrigin, colOrigin;
	int rowFrameSize, colFrameSize;
	int temp;
	int** imgAry;
	int** morphAry;
	int** structElemAry;
	char choice;
	
	morphology(int numRowsImg, int numColsImg, int minImg, int maxImg, int numRowsStructElem, int numColsStructElem, int minStructElem, int maxStructElem, int rowOrigin, int colOrigin){
		this->numRowsImg = numRowsImg;
		this->numColsImg = numColsImg;
		this->minImg = minImg;
		this->maxImg = maxImg;
		this->numRowsStructElem = numRowsStructElem;
		this->numColsStructElem = numColsStructElem;
		this->minStructElem = minStructElem;
		this->maxStructElem = maxStructElem;
		this->rowOrigin = rowOrigin;
		this->colOrigin = colOrigin;
		
		imgAry = new int*[numRowsImg + rowFrameSize];
		morphAry = new int*[numRowsImg + rowFrameSize];
		structElemAry = new int*[numRowsStructElem];
		
		computeFrameSize();
		
		for(int i = 0;i < numRowsImg + rowFrameSize; i++){
			imgAry[i] =  new int[numColsImg + colFrameSize];
			morphAry[i] =  new int[numColsImg + colFrameSize];
		}
		for(int i = 0; i < numRowsStructElem; i++){
			structElemAry[i] = new int[numColsStructElem];
		}
	}
	void computeFrameSize(){
		rowFrameSize = numRowsStructElem/2*2;
		colFrameSize = numColsStructElem/2*2;
	}
	void loadImage(string file){
		ifstream in;
		in.open(file.c_str());
		in>>temp>>temp>>temp>>temp;
		for(int i = rowFrameSize/2; i < numRowsImg + rowFrameSize/2; i++){
			for(int j = colFrameSize/2; j < numColsImg + colFrameSize/2; j++){
				in>>imgAry[i][j];
			}
		}
		in.close();
	}
	void zeroFrameImg(){
		for(int i = 0; i < numRowsImg + rowFrameSize; i++){
			for(int j = 0; j < numColsImg + colFrameSize; j++){
				imgAry[0][j] = 0;
				imgAry[numRowsImg + rowFrameSize - 1][j] = 0;
				imgAry[i][0] = 0;
				imgAry[i][numColsImg + colFrameSize - 1] = 0;
			}
		}
	}
	void loadStruct(string file){
		ifstream in;
		in.open(file.c_str());
		in>>temp>>temp>>temp>>temp;
		in>>rowOrigin>>colOrigin;
		for(int i = 0; i < numRowsStructElem; i++){
			for(int j = 0; j < numColsStructElem; j++){
				in>>structElemAry[i][j];
			}
		}
		in.close();
	}
	void consolePrettyPrint(string array){
		if(array == "imgAry"){
			cout<<"imgAry:"<<endl;
			for(int row = rowFrameSize/2; row<numRowsImg + rowFrameSize/2; row++){
				for(int col = colFrameSize/2; col < numColsImg + colFrameSize/2; col++){
					if(imgAry[row][col]==1)
						cout<<imgAry[row][col]<<" ";
					else cout<<"  ";
				}
				cout<<endl;
			}
			cout<<endl;
		}
		if(array == "structElemAry"){
			cout<<"structElemAry:"<<endl;
			for(int i = 0; i < numRowsStructElem; i++){
				for(int j = 0; j < numColsStructElem; j++){
					cout<<structElemAry[i][j]<<" ";
				}
				cout<<endl;
			}
			cout<<endl;
		}
		if(array == "morphAry"){
			cout<<"morphAry:"<<endl;
			for(int row = rowFrameSize/2; row<numRowsImg + rowFrameSize/2; row++){
				for(int col = colFrameSize/2; col < numColsImg + colFrameSize/2; col++){
					if(morphAry[row][col]==1)
						cout<<morphAry[row][col]<<" ";
					else cout<<"  ";
				}
				cout<<endl;
			}
			cout<<endl;
		}
	}
	void outFilePrettyPrint(ofstream& file){
		for(int row = rowFrameSize/2; row<numRowsImg + rowFrameSize/2; row++){
			for(int col = colFrameSize/2; col < numColsImg + colFrameSize/2; col++){
				if(morphAry[row][col]==1)file<<morphAry[row][col]<<" ";
				else file<<"  ";
			}
			file<<endl;
		}
		file<<endl;
	}
	void initMorphAry(){
		for(int i = 0;i < numRowsImg + rowFrameSize; i++){
			for(int j = 0;j < numColsImg + colFrameSize; j++){
				morphAry[i][j] = 0;
			}
		}
	}
	void initImgAry(){
		for(int i = 0;i < numRowsImg + rowFrameSize; i++){
			for(int j = 0;j < numColsImg + colFrameSize; j++){
				imgAry[i][j] = 0;
			}
		}
	}
	void MorphToImg(){
		for(int i = 0;i < numRowsImg + rowFrameSize; i++){
			for(int j = 0;j < numColsImg + colFrameSize; j++){
				imgAry[i][j] = morphAry[i][j];
			}
		}
	}
	void consoleRequest(char& choice){
		cout<<"Choose one of the following operations: "<<endl;
		cout<<"D: Dilation"<<endl;
		cout<<"E: Erosion"<<endl;
		cout<<"O: Opening"<<endl;
		cout<<"C: Closing"<<endl;
		cout<<"Q: Quit Program"<<endl;
		cout<<"Enter your choice(case sensitive): ";
		cin>>choice;
		while((choice!='D')&&(choice!='E')&&(choice!='O')&&(choice!='C')&&(choice!='Q')){
			cout<<endl<<"Invalid operation."<<endl<<"Enter a valid operation: ";
			cin>>choice;
		}
		cout<<endl;
		this->choice = choice;
	}
	void dilation(){
		for(int i = rowFrameSize/2; i < numRowsImg + rowFrameSize/2; i++){
			for(int j = colFrameSize/2; j < numColsImg + colFrameSize/2; j++){
				if(imgAry[i][j]==1){
					for(int row = -rowFrameSize/2; row < rowFrameSize/2+1; row++){
						for(int col = -colFrameSize/2; col < colFrameSize/2+1; col++){
							if(structElemAry[row+rowFrameSize/2][col+colFrameSize/2]!=0){
								morphAry[i+row][j+col] = structElemAry[row+rowFrameSize/2][col+colFrameSize/2];
							}
						}
					}
				}
			}
		}
	}
	void erosion(){
		int countStructPixels = 0;
		int countAryPixels = 0;
		for(int i = 0; i < numRowsStructElem; i++){
			for(int j = 0; j < numColsStructElem; j++){
				if(structElemAry[i][j]!=0)
					countStructPixels++;
			}
		}
		for(int i = rowFrameSize/2; i < numRowsImg + rowFrameSize/2; i++){
			for(int j = colFrameSize/2; j < numColsImg + colFrameSize/2; j++){
				if(imgAry[i][j]!=0){
					countAryPixels = 0;
					for(int row = -rowFrameSize/2; row < rowFrameSize/2+1; row++){
						for(int col = -colFrameSize/2; col < colFrameSize/2+1; col++){
							if(structElemAry[row+rowFrameSize/2][col+colFrameSize/2]!=0){
								if(imgAry[i+row][j+col]!=0){
									countAryPixels++;
								}
							}
						}
					}
					if(countStructPixels == countAryPixels){
						morphAry[i][j] = imgAry[i][j];
					}
				}
			}
		}
	}
	void opening(){
		erosion();
		initImgAry();
		MorphToImg();
		initMorphAry();
		dilation();
	}
	void closing(){
		dilation();
		initImgAry();
		MorphToImg();
		initMorphAry();	
		erosion();
	}
	void operation(char choice, ofstream& file){
		switch(choice){
			case 'D':
				dilation();
				consolePrettyPrint("morphAry");
				outFilePrettyPrint(file);
				return;
			case 'E':
				erosion();
				consolePrettyPrint("morphAry");
				outFilePrettyPrint(file);
				return;
			case 'O':
				opening();
				consolePrettyPrint("morphAry");	
				outFilePrettyPrint(file);							
				return;
			case 'C':
				closing();
				consolePrettyPrint("morphAry");
				outFilePrettyPrint(file);
				return;
			case 'Q':
				return;
		}
	}
};

int main(int argc, char* argv[]){
	ifstream inFile1 (argv[1]);
	string fileName1 = argv[1];
	ifstream inFile2 (argv[2]);
	string fileName2 = argv[2];
	ofstream outFile1 (argv[3]);
	int numRowsImg, numColsImg, minImg, maxImg;
	int numRowsStructElem, numColsStructElem, minStructElem, maxStructElem;
	int rowOrigin, colOrigin;
	char choice;

	if(inFile1.is_open() && inFile2.is_open()){
		inFile1>>numRowsImg>>numColsImg>>minImg>>maxImg;
		inFile2>>numRowsStructElem>>numColsStructElem>>minStructElem>>maxStructElem;
		inFile2>>rowOrigin>>colOrigin;
		morphology morph(numRowsImg, numColsImg, minImg, maxImg, numRowsStructElem, numColsStructElem, minStructElem, maxStructElem, rowOrigin, colOrigin);
		morph.loadImage(fileName1);
		morph.zeroFrameImg();
		morph.consolePrettyPrint("imgAry");
		morph.loadStruct(fileName2);
		morph.consolePrettyPrint("structElemAry");
		while(choice!='Q'){
			morph.initMorphAry();
			morph.consoleRequest(choice);
			morph.operation(choice,outFile1);
			morph.initImgAry();
			morph.MorphToImg();
		}
	}
	else cout<<"Couldn't retrieve data.";
	inFile1.close();
	inFile2.close();
	outFile1.close();
	return 0;
}
