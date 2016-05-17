#include "aco_path.h"

const float Ant::len_of_dir[] = { 1.414, 1, 1.414, 1, 1, 1.414, 1, 1.414, 0 };

const int Ant::move[][2] =
{
	-1,-1,
	0,-1,
	1,-1,
	-1,0,
	1,0,
	-1,1,
	0,1,
	1,1
};

ACO_PathPlan::ACO_PathPlan(int scale, int a_num, int max_t)
	:SIMethod(max_t,NULL), scale(scale), a_num(a_num)
{
	map = (bool**) new bool*[scale];
	for (int i = 0; i < scale; ++i)
	{
		map[i] = new bool[scale];
		for (int j = 0; j < scale; ++j)
		{
			map[i][j] = randGrid();
			if (map[i][j]) ++ob_num;
		}
	}
	if (map[0][0])
	{
		map[0][0] = 0;
		--ob_num;
	}
	if (map[scale - 1][scale - 1])
	{
		map[scale - 1][scale - 1] = 0;
		--ob_num;
	}

	grid_num = scale*scale;
	ants = new Ant[a_num];
	pher = new Pher[grid_num];
	note = new Pher[grid_num];
	pher_new = new Pher[grid_num];

	g_begin = 0;
	g_end = grid_num - 1;
	
	//printMap();
}

ACO_PathPlan::~ACO_PathPlan()
{
	for (int i = 0; i < scale; ++i)
	{
		delete[] map[i];
	}
	delete[]  map;
	delete[] ants;
	delete[] pher_new;
}

bool ACO_PathPlan::randGrid()
{
	float prob = 0.3;
	float r = rand() % 100 * 0.01;
	return (r < prob) ? true : false;
}

void ACO_PathPlan::printMap()
{
	for (int y = 0; y < scale; ++y)
	{

		for (int x = 0; x < scale; ++x)
			cout << map[x][y] << " ";
		cout << endl;
	}
}

int ACO_PathPlan::chooseNext(int x)
{
	int chosen_id = -1;
	int cur = ants[x].tabu.back();
	int chosen_dir = -1;
	if (Util::rand01() < q0)
	{
		float cmp = -1;

		for (int dir = 0; dir < 8; ++dir)
		{
			int j_id = xyId(
				xC(cur) + Ant::move[dir][0],
				yC(cur) + Ant::move[dir][1]);
			if (j_id<0 || j_id>scale*scale || map[xC(j_id)][yC(j_id)])
				continue;
			float tmp = pow(pher[cur][j_id], beta)*ita_function(j_id);
			if (tmp > cmp)
			{
				cmp = tmp;
				chosen_id = j_id;
				chosen_dir = dir;
			}
		}
	}
	else
	{
		float sum = 0;
		vector<int> options;
		vector<float> probs;
		for (int dir = 0; dir < 8; ++dir)
		{
			int j_id = xyId(
				xC(cur) + Ant::move[dir][0],
				yC(cur) + Ant::move[dir][1]);
			if (!is_valid_id(j_id))
				continue;
			options.push_back(dir);
			float tmp = pow(pher[cur][j_id], beta)*ita_function(j_id);
			sum += tmp;
			probs.push_back(tmp);
		}
		for (int i = 0; i < probs.size(); ++i)
		{
			probs[i] /= sum;
		}

		chosen_dir = options[Util::wheelGame(probs)];
		chosen_id = xyId(
			xC(cur) + Ant::move[chosen_dir][0],
			yC(cur) + Ant::move[chosen_dir][1]);
	}
	if (chosen_id == -1) throw;

	//if (ants[x].contain(chosen_id)) return -1;

	ants[x].choose(chosen_id, chosen_dir);

	pher_new[cur][chosen_id] =
		(1 - ro)*pher_new[cur][chosen_id] + ro*Q1 / ants[x].getCurrentLength();
	if (pher[cur][chosen_id] < Tmin)
		pher[cur][chosen_id] = Tmin;

	return chosen_id;
}

void ACO_PathPlan::step0()
{
	int begin = g_begin;
	for (int x = 0; x < a_num; ++x)
	{
		ants[x].choose(begin, -1);
		chooseNext(x);
	}
	swapPher();
}

void ACO_PathPlan::iterate_do()
{
	restore();
	step0();
	while (true)
	{
		
		int count = 0;
		for (int x = 0; x < a_num; ++x)
		{
			if (ants[x].tabu.back() == g_end)
				continue;
			int choose = chooseNext(x);
			if (choose == g_end)
			{
				ants[x].is_done = true;
				for (int c = 0; c < ants[x].tabu.size() - 1; ++c)
				{
					note[ants[x].tabu[c]][ants[x].tabu[c + 1]]
						+= Q2 / ants[x].cur_length;
				}

				if (ants[x].cur_length < gbest.cur_length || gbest.cur_length == 0)
				{
					gbest.assign(&ants[x]);
				}
			}
			else
			{
				++count;
			}
			//ants[0].print();
			//system("pause");
		}
		if (count != 0) continue;

		evap_pher(alpha);

		break;
	}
	swapPher();

	//gbest.print();
}

void ACO_PathPlan::save_gbest(const char * name)
{
	fstream out(name, ios::out);
	out << name << endl;
	out << "Current Global Best Value: " << gbest.cur_length << endl;
	out << "Current Global Best Rotine:" << endl;

	for (int i = 0; i < gbest.tabu.size(); ++i)
	{
		out << gbest.tabu[i] << endl;
	}

	out.close();
}

void ACO_PathPlan::evap_pher(float f)
{
	for (int id = 0; id < grid_num; ++id)
	{
		for (int dir = 0; dir < 8; ++dir)
		{
			int tar = xyId(
				xC(id) + Ant::move[dir][0],
				yC(id) + Ant::move[dir][1]);
			if (!is_valid_id(tar)) continue;
			pher[id][tar] = pher[id][tar] * f + (1 - f)*note[id][tar];
		}
	}
}

inline void ACO_PathPlan::swapPher()
{
	delete[] pher;
	delete[] note;
	pher = pher_new;
	note = new Pher[grid_num];
	pher_new = new Pher[grid_num];
}

inline void ACO_PathPlan::restore()
{
	for (int x = 0; x < a_num; ++x)
	{
		ants[x].clear();
	}
}

inline bool ACO_PathPlan::is_valid_id(int j_id)
{
	return (j_id>0 && j_id<scale*scale && !map[xC(j_id)][yC(j_id)]);
}

float ACO_PathPlan::ita_function(int i)
{
	float dist = Util::dist(xC(i), yC(i), xC(g_end), yC(g_end));
	if (Util::equalf(dist, 0))
		dist = 0.000001;
	return D / dist;
}

int ACO_PathPlan::xC(int i)
{
	return (i) % scale;
}

int ACO_PathPlan::yC(int i)
{
	return (i) / scale;
}

int ACO_PathPlan::xyId(int x, int y)
{
	if (x < 0 || x >= scale || y < 0 || y >= scale)
		return -1;
	return y*scale + x;
}

void ACO_PathPlan::getRouteArr(vector<int> &vec, int x)
{
	vec.clear();
	Ant *antp;
	antp = (x == -1) ? &gbest : &ants[x];
	WaitForSingleObject(antp->mutex, INFINITE);
	for (int i = 0; i < antp->tabu.size(); ++i)
	{
		vec.push_back(xC(antp->tabu[i]));
		vec.push_back(yC(antp->tabu[i]));
	}
	ReleaseMutex(antp->mutex);
}