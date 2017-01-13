#include "I want 7.h"
#include <random>
#include <functional>
#include <algorithm>
#include <list>

default_random_engine _gene;

//NumBlock ��������

NumBlock::NumBlock()
{

	uniform_int_distribution<int> dist_num(0, 6);
	binomial_distribution<int> dist_lock(1, 0.1);

	auto num_rand = bind(dist_num, _gene);
	auto lock_rand = bind(dist_lock, _gene);
	number = rand()%7;
	active = false;
	lock = (rand() % 100 > 90) ? 1 : 0;
	//lock = 0;
}

NumBlock::NumBlock(int num, bool act, unsigned int locknum) :number(num), active(act), lock(locknum)
{
}

NumBlock::NumBlock(const NumBlock & block)
{
	number = block.getNumber();
	lock = block.getLock();
	active = block.isActive();
}

NumBlock & NumBlock::operator=(const NumBlock & block)
{
	number = block.getNumber();
	lock = block.getLock();
	active = block.isActive();
	return *this;
}

bool NumBlock::operator==(const NumBlock & second) const
{
	return this->getNumber() == second.getNumber() && this->getLock() == second.getLock();;
}

bool NumBlock::isActive()const
{
	return active;
}

unsigned int NumBlock::getLock()const
{
	return lock;
}

unsigned int NumBlock::getNumber()const
{
	return number;
	return 0;
}

NumBlock & NumBlock::setlock(unsigned int locknum)
{
	lock = locknum;
	return *this;
}

NumBlock& NumBlock::setActive(bool state)
{
	active = state;
	return *this;
}

NumBlock & NumBlock::unlock()
{
	// TODO: �ڴ˴����� return ���
	if (lock != 0) {
		--lock;
	}
	active = true;
	return *this;
}

bool NumBlock::isAbsorbable(const NumBlock & side)const
{
	if (number == 0) {//0 ���ܳ�Ϊ����������,ֻ�ܱ�7����
		return false;
	}
	else if (number == 7) {
		return side.getLock() == 0 && side.getNumber() == 0;
	}
	else {
		return side.getLock() == 0 && this->lock == 0
			&& side.getNumber() + number == 7;
	}//�����Ķ����ܱ����պ���������������
}

ostream & operator<<(ostream & out, const NumBlock& block)
{
	out << ' ';
	char decorator = ' ';
	switch (block.lock) {
	case 0:decorator = ' '; break;
	case 1:decorator = '_'; break;
	case 2:decorator = '^'; break;
	default:decorator = '*'; break;
	}

	if (block.number == 7 || (block.number < 7 && block.lock != 1)) {
		out << decorator << block.number << decorator;
	}
	else if (block.number < 7 && block.lock == 1) {
		out << '(' << block.number << ')';
	}
	else {
		out << "   ";
	}

	out << ' ';
	return out;
}


//BlockPair ��������

void BlockPair::_init()
{
	pair[0] = NumBlock();
	do {
		pair[1] = NumBlock();
	} while ((pair[1].isAbsorbable(pair[0])));//���ɿ��Թ�������� NumBlock
	direction = WEST;
	firstloc = 2;
	secondloc = 3;
}

BlockPair::BlockPair()
{
	_init();
}

BlockPair::BlockPair(NumBlock & first, NumBlock &second)
{
	pair[0] = first;
	pair[1] = second;
	direction = WEST;
	firstloc = 2;
	secondloc = 3;
}

void BlockPair::Random()
{
	_init();//�ٴε���_init
}

void BlockPair::setEmpty()
{
	pair[0] = nullBlock;
	pair[1] = nullBlock;
	direction = WEST;
	firstloc = 2;
	secondloc = 3;
}


BlockPair::Direction BlockPair::getDirection()const
{
	return direction;
}

NumBlock& BlockPair::getFirst()
{
	return pair[0];
}

NumBlock& BlockPair::getSecond()
{
	return pair[1];
}

unsigned int BlockPair::getFirstLoc()const
{
	return firstloc;
}

unsigned int BlockPair::getSecondLoc()const
{
	return secondloc;
}


BlockPair & BlockPair::turn()
{//ת�����޸�firstloc��secondloc ��ֵ
	direction = Direction((direction + 1) % 4);
	switch (direction) {
	case NORTH:firstloc = secondloc; break;
	case WEST:if (firstloc == 0) { secondloc = firstloc + 1; }
			  else { firstloc = secondloc - 1; }break;
	case SOUTH:firstloc = secondloc; break;
	case EAST:if (firstloc == 0) { firstloc = secondloc + 1; }
			  else { secondloc = firstloc - 1; }break;
	default:break;
	}
	return *this;
}

void BlockPair::print(ostream & out)const
{
	
	if (direction == NORTH) {
		out << "\t ";
		for (unsigned int i = 0; i < firstloc; ++i)
			out << nullBlock << ' ';
		out << pair[0] << ' ';
		out << endl << "\t ";
		for (unsigned int i = 0; i < secondloc; ++i)
			out << nullBlock << ' ';
		out << pair[1];
		out << endl;
	}
	if (direction == SOUTH) {
		out << "\t ";
		for (unsigned int i = 0; i < secondloc; ++i)
			out << nullBlock << ' ';
		out << pair[1] << ' ';
		out << endl << "\t ";
		for (unsigned int i = 0; i < firstloc; ++i)
			out << nullBlock << ' ';
		out << pair[0];
		out << endl;
	}
	if (direction == WEST) {
		out << endl << "\t ";
		for (unsigned int i = 0; i < firstloc; ++i)
			out << nullBlock << ' ';
		out << pair[0] << ' ';
		out << pair[1];
		out << endl;
	}
	if (direction == EAST) {
		out << endl << "\t ";
		for (unsigned int i = 0; i < secondloc; ++i)
			out << nullBlock << ' ';
		out << pair[1] << ' ';
		out << pair[0];
		out << endl;
	}
}

void BlockPair::move(char dir)
{//'a'Ϊ����,'d'Ϊ����,�����˱߽����
	if (dir == 'a' && firstloc != 0 && secondloc != 0) {
		--firstloc;
		--secondloc;
	}
	else if (dir == 'd' && firstloc != 5 && secondloc != 5) {
		++firstloc;
		++secondloc;
	}
}


//GameMap ��������

GameMap::GameMap():score(0)
{
}

int GameMap::getScore() const
{
	return score;
}

void GameMap::insert(NumBlock & block, unsigned int col)
{
	if (col > 5) {
		return;
	}
	map[col].push_back(block);
}

//ʹ����find����
bool GameMap::trim()
{
	bool flag = false;
	vector<NumBlock>::iterator firstnull;

	for (auto columniter = map.begin(); columniter != map.end(); ++columniter) {
		firstnull = find(columniter->begin(), columniter->end(), nullBlock);//�ҵ�ÿһ�еĵ�һ��ռλ��,��find()����
		while (firstnull != columniter->end()) {//��֮���ÿһ����в���
			if (*firstnull == nullBlock) {//���Ϊ ռλ�� ��ɾ��
				firstnull = columniter->erase(firstnull);
			}
			else {//�����Ϊ�տ�,������Ϊ�״̬(�䷢�������䶯��)
				(*(firstnull++)).setActive(true);
				flag = true;
			}
		}
	}
	return flag;
}

bool GameMap::clearSeven()
{
	bool flag = false;
	vector<group> clearqueue;//��¼�������ȥ�� group
	vector<vector<NumBlock>::iterator> temp;//��¼��ǰ�������м��������� 7 ��
	int count = 0;

	for (unsigned int col = 0; col != map.size(); ++col) {
		for (unsigned int row = 0; row != map[col].size(); ++row) {
			if (map[col][row].getNumber() != 7)
				continue;

			temp.clear();

			beside(col, row, [](vector<NumBlock>::iterator blockiter) {
				return blockiter->getNumber() == 7;
			}, [&](unsigned int col, unsigned int row) {
				temp.push_back(index(col, row));
			});//�������������Χ�ĸ���,���Ҳ�� 7 ���¼���Ե�ǰ��Ϊ���Ŀ��group��

			if (temp.size() >= 2)//���temp���������������ȥ
				clearqueue.push_back(group(index(col, row), temp, this));
		}
	}

	for (auto group : clearqueue) {
		count += group.clear();//����group����ȥ����
		flag = true;
	}
	score += ((1 + count)*count) / 2;//�����ı�

	setAllUnActive();

	return flag;
}

//ʹ����find_if����
bool GameMap::absorb()
{
	bool flag = false;
	list<group> absorbqueue;
	vector<vector<NumBlock>::iterator> temp;

	for (unsigned int col = 0; col != map.size(); ++col) {
		for (unsigned int row = 0; row != map[col].size(); ++row) {

			vector<NumBlock>::iterator first = index(col, row);
			temp.clear();

			beside(col, row, [&first](vector<NumBlock>::iterator& second) {
				return first->isAbsorbable(*second);
			}, [&](unsigned int col, unsigned int row) {
				temp.push_back(index(col, row));
			});//�ҵ��������Χ�����п������յĿ�,ע�⵱ǰ�鲻һ����������

			if (temp.size() > 0) {
				group cur{ first,temp,this };

				auto iter = find_if(absorbqueue.begin(), absorbqueue.end(), [&cur](const group& second) {
					return second < cur;
				});
				absorbqueue.insert(iter, cur);
				//�����ȼ��ߵ�group������������,������α���������
			}
		}
	}

	for (auto group: absorbqueue) {
		group.absorb();
		flag = true;
	}

	setAllUnActive();
	return flag;
}

bool GameMap::isOutOfRange()
{
	for (auto column : map) {
		if (column.size() > 9) {
			return true;
		}
	}
	return false;
}

GameMap::Function GameMap::execute(Function funcname)
{//����״̬��ת�ƽ��к����ĵ���,ת��ͼ�� ����˵��
	switch (funcname) {
	case BEGIN:if (absorb())return TRIM; else return CLEAR;
	case ABSORB:if (absorb())return TRIM; else return CLEAR;
	case TRIM:if (trim())return ABSORB; else return CLEAR;
	case CLEAR:if (clearSeven())return TRIM; else return END;
	case END:return END;
	default:return END;
	}
}

void GameMap::print(ostream& out) {
	for (int row = 8; row > -1; --row) {
		out << "\t|";
		for (unsigned int col = 0; col < 6; ++col) {
			if (map[col].size() >(unsigned int)row) {
				out << map[col][row];
			}
			else {
				out << nullBlock;
			}
			out << "|";
		}
		out << endl;
	}
	cout << "\t" << "------------------------------------" << endl;
	out << endl;
	out << "\tscore: " << score << endl;
}

void GameMap::setAllUnActive()
{
	for (auto coliter = map.begin(); coliter != map.end(); ++coliter) {
		for (auto rowiter = coliter->begin(); rowiter != coliter->end(); ++rowiter) {
			rowiter->setActive(false);
		}
	}
}

template<typename Judge, typename Body>
void GameMap::beside(unsigned int col, unsigned int row, Judge isValid, Body conduct)
{
	if (col > 5 || row >= (map[col]).size()) {
		return;
	}

	if (row > 0 && isValid(index(col, row - 1))) {
		conduct(col, row - 1);
	}
	if (row < map[col].size() - 1 && isValid(index(col, row + 1))) {
		conduct(col, row + 1);
	}
	if (col > 0 && row < map[col - 1].size() && isValid(index(col - 1, row))) {
		conduct(col - 1, row);
	}
	if (col < map.size() - 1 && row < map[col + 1].size() && isValid(index(col + 1, row))) {
		conduct(col + 1, row);
	}
}

template<typename Judge, typename Body>
void GameMap::beside(vector<NumBlock>::iterator iter, Judge isValid, Body conduct)
{
	unsigned int row = 0;
	unsigned int col = 0;
	for (auto columniter = map.begin(); columniter != map.end(); ++columniter) {
		for (auto blockiter = columniter->begin(); blockiter != columniter->end(); ++blockiter) {
			if (blockiter == iter) {
				col = columniter - map.begin();
				row = blockiter - columniter->begin();
			}
		}
	}//�ֽ�iter�õ�����,ֻ��ͨ����������ҵ���Χ��
	beside(col, row, isValid, conduct);
}


//GameMap::group ��������

int GameMap::group::clear() {
	neighbor.push_back(center);//ɾȥ����Ϊ7�Ŀ�
	int count = 0;

	for (auto blockiter = neighbor.begin(); blockiter != neighbor.end(); ++blockiter) {
		if (!(*(*blockiter) == nullBlock)) {
			count += (*blockiter)->getLock() + 1;
			(*(*blockiter)) = nullBlock;
		}

		map->beside(*blockiter, [](const vector<NumBlock>::iterator& second) {
			return second->getNumber() != 7 && second->getLock() != 0;
		}, [&](unsigned int col, unsigned int row) {
			map->index(col, row)->unlock();
		});//������ǰ�������,�����н�������,�˴��õ���GameMap��ָ��
	}
	return count;
}

//ʹ����for_each��find_if����
void GameMap::group::absorb() {

	if ((!(*center == nullBlock)) && find_if(neighbor.begin(), neighbor.end(), [](const vector<NumBlock>::iterator& seciter) {
		return *seciter == nullBlock;
	}) == neighbor.end()) {//���Ĳ��ǿ�,�������ڽ�������û�п�
		for_each(neighbor.begin(), neighbor.end(), [](vector<NumBlock>::iterator& seciter) {
			*seciter = nullBlock;
		});
		if (center->getNumber() != 7) {
			*center = NumBlock(7, true, neighbor.size() - 1);
		}
	}
}

bool GameMap::group::operator<(const group& second)const 
{//�Ƚϵ��㷨 �� ����˵��
	if (this->neighbor.size() < second.neighbor.size()) {
		return true;
	}
	else if (this->neighbor.size() == second.neighbor.size() && !this->center->isActive()) {
		return true;
	}
	else {
		return false;
	}
}

//ʹ����copy����
GameMap::group::group(vector<NumBlock>::iterator& cen, vector<vector<NumBlock>::iterator>& neigh, GameMap* gmap) :map(gmap) {
	center = cen;
	copy(neigh.begin(), neigh.end(), inserter(neighbor, neighbor.begin()));//ʹ����copy���� �� inserter
}


//ȫ�ֺ�������

bool operator==(const vector<NumBlock>::iterator& liter, const vector<NumBlock>::iterator& riter) {
	return *liter == *riter;
}

void fall(BlockPair& bp, GameMap& gmp) {
	bp.getFirst().setActive(true);
	bp.getSecond().setActive(true);
	if (bp.getDirection() == BlockPair::Direction::NORTH) {
		gmp.insert(bp.getSecond(), bp.getSecondLoc());
		gmp.insert(bp.getFirst(), bp.getFirstLoc());
	}
	else {
		gmp.insert(bp.getFirst(), bp.getFirstLoc());
		gmp.insert(bp.getSecond(), bp.getSecondLoc());
	}
	bp.setEmpty();
}

void print(ostream& out, BlockPair& bp, GameMap& gmp) {
	out << "Game Begin" << endl << endl;
	bp.print(out);
	cout << "\t" << "------------------------------------" << endl;
	gmp.print(out);
	out << endl;
	out << "\tinput:\t";
	
}
