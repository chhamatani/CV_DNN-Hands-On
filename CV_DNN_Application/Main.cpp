// CV_DNN_Application.cpp : アプリケーションのエントリ ポイントを定義します。
//
#include "AlexNetClassifier.h"
#include "GoogLeNetClassifier.h"
#include "Paths.h"

#include<memory>
#include "UserCommunicator.h"
#include <sys/stat.h>

using namespace std;
using namespace OpenCVApp;

int main(int argc, char **argv)
{
	unique_ptr<IClassifier> dnnClassifier = nullptr;
	
	try {
		std::string fileName = (argc > 1) ? argv[1] : UserCommunicator::askForFilename();		
		cv::Mat image = cv::imread(fileName, 1);
		
		dnnClassifier = unique_ptr<IClassifier>(new AlexNetClassifier());
	
		/* 1. ニューラルネットワークを形成する。 */
		dnnClassifier->createNeuralNet();

		/* 2. 入力層に画像をセットする。 */
		dnnClassifier->setImage(&image);

		/* 3. 画像分類を行う。(出力層まで順伝播させる。) */
		cv::Mat result;
		dnnClassifier->classify(&result);

		_wmkdir(L"layers");
		dnnClassifier->outLayerAsImage("conv1", "layers\\conv1_", ".png");

		int classId;
		double classProb;
		
		IClassifier::getMaxClass(&result, &classId, &classProb); 
		std::vector<std::string> classNames = IClassifier::readClassNames( Paths::MODEL_DIR + "\\synset_words.txt");
		std::cout << "Best class: #" << classId << " '" << classNames.at(classId) << "'" << std::endl;
		std::cout << "Probability: " << classProb * 100 << "%" << std::endl;
	}
	catch (cv::Exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
