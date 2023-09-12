#include "BPNet.h"

bool BPNet::initialize(int therow, int thecol, double theeta, int max, Solver tsolver, int loss_iter)
{
	net_row = therow;
	net_col = thecol;
	eta = theeta;
	max_iter = max;
	loss_threshold = loss_iter;
	thesolver = tsolver;

	beta_1 = 0.9;
	beta_2 = 0.999;
	epsilon = 1e-8;


	input.resize(thecol + 2 + 1);
	output.resize(thecol + 2);
	weight.resize(thecol + 2);
	weight_delta.resize(thecol + 2);
	bias.resize(thecol + 2);
	delta.resize(thecol + 2);
	m_t.resize(thecol + 2);
	v_t.resize(thecol + 2);

	for (int i = 1; i < thecol + 1; ++i)
	{
		input[i] = VectorXd::Constant(therow, 0);
		output[i] = VectorXd::Constant(therow, 0);
		weight[i] = MatrixXd::Constant(therow, therow, 0);
		weight_delta[i] = MatrixXd::Constant(therow, therow, 0);
		bias[i] = VectorXd::Constant(therow, 0);
		delta[i] = VectorXd::Constant(therow, 0);

		m_t[i] = MatrixXd::Constant(therow, therow, 0);
		v_t[i] = MatrixXd::Constant(therow, therow, 0);
	}

	//sta = manager->getSta(img);
	//sta->initialize();
	//mean_table = sta->meanVec();
	//cor_mat = sta->coVarMat();

	return true;
}

bool BPNet::train(vector<vector<VectorXd>>* thesample)
{
	sample = thesample;
	initialize_io();
	randomWeight();
	calSampleMean();
	calSampleVar();

	for (int k = 0; k < max_iter; ++k)
	{
		int sum = 0;
		for (int i = 0; i < (*sample).size(); ++i)
		{
			for (int j = 0; j < (*sample)[i].size() / 2; ++j)
			{
				calForward((*sample)[i][j]);
				calBackward(i);
				sum++;
			}
		}
		for (int i = 0; i < (*sample).size(); ++i)
		{
			for (int j = (*sample)[i].size() / 2; j < (*sample)[i].size(); ++j)
			{
				calForward((*sample)[i][j]);
				calBackward(i);
				sum++;
			}
		}


		loss /= sum;
		loss = -loss;
		//eta = eta * 1.0001;
		//eta = eta * exp(-0.001 * k);

		cout << "Epoch: " << k << endl << "Loss: " << loss << endl << endl;
	}

	return true;
}

Image* BPNet::classify(string& opath)
{
	result = manager->create();
	result->create(1, img->getRow(), img->getCol(), img, opath);

	int t2;

	for (int i = 0; i < img->getBlockRow(); ++i)
	{
		for (int j = 0; j < img->getBlockCol(); ++j)
		{
			img->loadDatabyBlock(i, j);
			result->prepareDatabyBlock(i, j, 0);

			for (int a = 0; a < img->getEachBlockRow(); ++a)
			{
				for (int b = 0; b < img->getEachBlockCol(); ++b)
				{
					VectorXd temp(3);
					//for (int c = 0; c < img->getBand(); ++c)
					{
						temp(0) = img->atBlock(2, a, b);
						temp(1) = img->atBlock(3, a, b);
						temp(2) = img->atBlock(4, a, b);
					}

					t2 = calForward(temp);
					result->setBlock(0, a, b, t2);

				}
			}
			//int t3 = result->atBlock(0, i, j);
			result->saveDatabyBlock(i, j, 0);
			//result->loadDatabyBlock(i, j, 0);
			//int t4 = result->atBlock(0, i, j);
			result->clear();
			img->clear();
			cout << "completed " << i * img->getBlockRow() + j << " block" << endl;
		}
	}

	return result;

}

MatrixXd* BPNet::getConfusionMatrix(vector<vector<VectorXd>>* thesample)
{
	test = thesample;
	confusion_mat = MatrixXd::Constant(test->size(), test->size(), 0);

	for (int i = 0; i < test->size(); ++i)
	{
		for (int j = 0; j < (*test)[i].size(); ++j)
		{
			confusion_mat(i, calForward((*test)[i][j]))++;
		}
	}

	return &confusion_mat;
}


bool BPNet::initialize_io()
{
	features = (*sample)[0][0].rows();
	input[0] = VectorXd::Constant(features, 0);
	output[0] = VectorXd::Constant(net_row, 0);
	weight[0] = MatrixXd::Constant(net_row, features, 0);
	weight_delta[0] = MatrixXd::Constant(net_row, features, 0);
	bias[0] = VectorXd::Constant(net_row, 0);
	delta[0] = VectorXd::Constant(net_row, 0);
	m_t[0] = MatrixXd::Constant(net_row, features, 0);
	v_t[0] = MatrixXd::Constant(net_row, features, 0);

	int types = sample->size();
	input[net_col + 1] = VectorXd::Constant(net_row, 0);
	output[net_col + 1] = VectorXd::Constant(types, 0);
	weight[net_col + 1] = MatrixXd::Constant(types, net_row, 0);
	weight_delta[net_col + 1] = MatrixXd::Constant(types, net_row, 0);
	bias[net_col + 1] = VectorXd::Constant(types, 0);
	delta[net_col + 1] = VectorXd::Constant(types, 0);
	m_t[net_col + 1] = MatrixXd::Constant(types, net_row, 0);
	v_t[net_col + 1] = MatrixXd::Constant(types, net_row, 0);

	input[net_col + 2] = VectorXd::Constant(types, 0);

	return true;
}

bool BPNet::randomWeight()
{
	double var = sqrt(2 / double(net_row * 2));
	double varlast = sqrt(2 / double(net_row + sample->size()));

	for (int i = 0; i < weight.size() - 1; ++i)
	{
		for (int j = 0; j < weight[i].rows(); ++j)
		{
			for (int k = 0; k < weight[i].cols(); ++k)
			{
				weight[i](j, k) = normDis(var);
			}
		}
	}

	for (int i = 0; i < weight[net_col + 1].rows(); ++i)
	{
		for (int j = 0; j < weight[net_col + 1].cols(); ++j)
		{
			weight[net_col + 1](i, j) = normDis(varlast);
		}
	}

	for (int i = 0; i < bias.size(); ++i)
	{
		for (int j = 0; j < bias[i].rows(); ++j)
		{
			bias[i](j) = 0.1;//normDis(1);
		}
	}


	return true;
}

double BPNet::normDis(double var)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::default_random_engine generator(seed);
	// 第一个参数为高斯分布的平均值，第二个参数为标准差
	std::normal_distribution<double> distribution(0.0, var);

	return distribution(generator);
}

bool BPNet::equalize(VectorXd& the)
{
	for (int i = 0; i < the.rows(); ++i)
	{
		the(i) = (the(i) - mean_vec(i)) / var_vec(i);
	}

	return true;
}

bool BPNet::calSampleMean()
{
	mean_vec = VectorXd::Constant(features, 0);
	int sum = 0;

	for (int i = 0; i < (*sample).size(); ++i)
	{
		for (int j = 0; j < (*sample)[i].size(); ++j)
		{
			for (int k = 0; k < features; ++k)
			{
				mean_vec(k) += (*sample)[i][j](k);
			}
			sum++;
		}
	}

	for (int k = 0; k < features; ++k)
	{
		mean_vec(k) /= sum;
	}

	return true;
}

bool BPNet::calSampleVar()
{
	var_vec = VectorXd::Constant(features, 0);
	int sum = 0;
	for (int i = 0; i < var_vec.size(); ++i)
	{
		for (int j = 0; j < (*sample)[i].size(); ++j)
		{
			for (int k = 0; k < features; ++k)
			{
				var_vec(k) += ((*sample)[i][j](k) - mean_vec(k)) * ((*sample)[i][j](k) - mean_vec(k));
			}
			sum++;
		}
	}

	for (int k = 0; k < features; ++k)
	{
		var_vec(k) /= (sum - 1);
	}

	for (int k = 0; k < features; ++k)
	{
		var_vec(k) = sqrt(var_vec(k));
	}

	return true;
}

int BPNet::calForward(VectorXd& theinput)
{
	//equalize(theinput);
	input[0] = theinput;
	//cout << "input"<<endl<<input[0] << endl;
	for (int i = 0; i < weight.size(); ++i)
	{
		output[i] = weight[i] * input[i] + bias[i];
		//cout << "wx+b:"<<endl<<output[i] << endl;
		activation(i);
		//cout <<"f(x)"<<endl<< input[i + 1] << endl;
	}

	out = input[input.size() - 1];

	double max = -100000;
	int pos = 0;
	for (int i = 0; i < out.rows(); ++i)
	{
		if (max < out(i))
		{
			max = out(i);
			pos = i;
		}
	}

	return pos;
}

bool BPNet::activation(int k)
{
	for (int i = 0; i < output[k].rows(); ++i)
	{
		input[k + 1](i) = 1 / (1 + exp(-output[k](i)));
	}

	return true;
}

bool BPNet::activation(VectorXd& the)
{
	for (int i = 0; i < the.rows(); ++i)
	{
		the(i) = 1 / (1 + exp(-the(i)));
	}

	return false;
}

VectorXd BPNet::derivative(VectorXd& the)
{
	VectorXd temp;
	temp = VectorXd::Constant(the.rows(), 1);
	temp -= the;
	temp = temp.array() * the.array();
	//cout << "deriva:" << endl << temp << endl;
	return temp;
}

bool BPNet::calBackward(int tru)
{
	VectorXd truth;
	truth = VectorXd::Constant(sample->size(), 0);
	truth(tru) = 1;

	calDelta(truth);
	updateWeight();

	return true;
}

bool BPNet::calDelta(VectorXd& truth)
{
	//cout << "Truth: " << endl << truth << endl << "output: " << endl << out << endl << endl;

	//首先计算输出层误差
	true_delta = out - truth;

	calLoss(true_delta);

	VectorXd deriv = derivative(out);
	delta[weight.size() - 1] = true_delta;// .array()* deriv.array();
	//cout << "delta:" << endl << delta[weight.size() - 1] << endl;

	//在计算隐藏层误差
	for (int i = weight.size() - 2; i >= 0; i--)
	{
		//cout <<"input"<<endl<< input[i + 1] << endl;
		VectorXd der = derivative(input[i + 1]);
		//cout << "der:" << endl << der << endl;
		MatrixXd weidelta = (weight[i + 1].transpose()) * delta[i + 1];
		//cout <<"weigh delta"<<endl<< weidelta << endl;
		delta[i] = weidelta.array() * der.array();
		//cout << "delta: " << i << endl << delta[i] << endl << endl;
	}

	return true;
}

bool BPNet::updateWeight()
{
	for (int i = 0; i < weight.size(); ++i)
	{
		MatrixXd tempgrad, tempgrad2, diaggrad, m_t_, v_t_, temp3;


		switch (thesolver)
		{
		case sgd:
			weight_delta[i] = eta * (delta[i] * (input[i].transpose()));
			//cout << weight_delta[i] << endl;
			break;
		case adam:
			//cout << delta[i] << endl;
			//cout << input[i] << endl;

			tempgrad = (delta[i] * (input[i].transpose()));
			tempgrad2 = tempgrad.array() * tempgrad.array();

			diaggrad = MatrixXd::Constant(tempgrad2.rows(), tempgrad2.cols(), 0);
			for (int i = 0; i < diaggrad.rows(); ++i)
			{
				for (int j = 0; j < diaggrad.cols(); ++j)
				{
					diaggrad(i, i) += tempgrad2(i, j);
				}
			}

			//cout << tempgrad << endl;
			//cout << tempgrad2 << endl;

			m_t[i] = beta_1 * m_t[i] + (1 - beta_1) * tempgrad;
			v_t[i] = beta_2 * v_t[i] + (1 - beta_2) * tempgrad2;

			//cout << m_t[i] << endl;
			//cout << v_t[i] << endl;

			m_t_ = MatrixXd::Constant(m_t[i].rows(), m_t[i].cols(), 0);
			v_t_ = MatrixXd::Constant(v_t[i].rows(), v_t[i].cols(), 0);

			for (int a = 0; a < m_t_.rows(); ++a)
			{
				for (int b = 0; b < m_t_.cols(); ++b)
				{
					//t1 = m_t[i](a, b);
					//t2 = (1 - pow(beta_1, currentEpoch + 1));
					m_t_(a, b) = m_t[i](a, b) / (1 - pow(beta_1, currentEpoch + 1));
					//t1 = m_t_(a, b);
					v_t_(a, b) = v_t[i](a, b) / (1 - pow(beta_2, currentEpoch + 1));
					//t2 = v_t_(a, b);
					v_t_(a, b) = sqrt(v_t_(a, b)) + epsilon;
				}
			}

			//cout << m_t_ << endl;
			//cout << v_t_ << endl;

			temp3 = m_t_.array() / v_t_.array();
			//cout << temp3 << endl;
			//cout << eta << endl;
			weight_delta[i] = eta * temp3;
			//cout << weight_delta[i];
			break;
		case sgdm:
			tempgrad = (delta[i] * (input[i].transpose()));
			m_t[i] = beta_1 * m_t[i] + eta * tempgrad;
			weight_delta[i] = m_t[i];
			break;
		default:
			break;
		}

		weight[i] -= weight_delta[i];
		//cout << "weight" << endl << weight[i] << endl;
		bias[i] -= eta * delta[i];
		//cout << "bias" << endl << bias[i] << endl;
	}

	return true;
}

bool BPNet::calLoss(VectorXd& t)
{
	double temp = 0;
	for (int i = 0; i < t.rows(); ++i)
	{
		//temp += (t(i) * t(i));
		temp += (t(i) * log(out(i)) + (1 - t(i)) * log(1 - out(i)));
	}
	loss += temp;

	return true;
}
