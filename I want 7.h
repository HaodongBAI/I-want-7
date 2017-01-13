#pragma once
#include <iostream>
#include <vector>
#include <array>
using namespace std;

//��Ϸ�е� �� ��
class NumBlock {
private:
	unsigned int number;//�������
	bool active;//���Ƿ��Ծ,����һ���Ƿ����ı�,��GameMap::absorb()��ʹ��
	unsigned int lock;//��ļ������,���������7���ʾ�������
	NumBlock& setlock(unsigned int);
public:
	NumBlock();//������ֹ����
	NumBlock(int, bool, unsigned int);
	NumBlock(const NumBlock&);//��������
	NumBlock& operator=(const NumBlock&);
	bool operator==(const NumBlock&)const;

	bool isActive()const;
	unsigned int getNumber()const;
	unsigned int getLock()const;

	NumBlock& setActive(bool);
	NumBlock& unlock();//����,��һ�������¿��Զ�����н���,lock��һ
	bool isAbsorbable(const NumBlock&)const;//�Ƿ�ɶ� ������ ��������

	friend ostream& operator<<(ostream&, const NumBlock&);//�����
};

//��̬�� �տ� ����,������Ϸ���������λ��
static const NumBlock nullBlock(8, false, 0);


//��Ϸ�е� ����� ��,�����������NumBlock���,����¼�����߹�ϵ
class BlockPair {
public:
	//�Զ����ö������,��¼�ڶ���������ڵ�һ�����λ��
	enum Direction
	{
		NORTH = 0,
		EAST = 1,
		SOUTH = 2,
		WEST = 3,
	};

	BlockPair();
	BlockPair(NumBlock&, NumBlock&);
	void Random();
	void setEmpty();
	Direction getDirection()const;
	NumBlock& getFirst();
	NumBlock& getSecond();
	unsigned int getFirstLoc()const;
	unsigned int getSecondLoc()const;

	BlockPair& turn();//ת�� ����� �ķ���
	void print(ostream&)const;
	void move(char);//�ƶ� ����� ��map�е�λ��
private:
	array<NumBlock, 2> pair;
	Direction direction;
	unsigned int firstloc;//��¼��һ��NumBlock��λ��
	unsigned int secondloc;//��¼�ڶ���NumBlock��λ��
	void _init();//��ʼ��,�õ��������Թ����NumBlock
};

//��Ϸ�е� ��ͼ ��,������һ����ά����
class GameMap {
private:
	array<vector<NumBlock>, 6> map;//�����ͼ,��6�к��8��,����array<,6>��¼��,vector��¼��
	int score;//��¼����
private:
	//�ڲ�����,������Ҫ���߼�����,��execut()��������
	bool trim();//���к���: ��� �տ�(nullBlock,ռλ��), ���û�п�������� �� �򷵻� false
	bool clearSeven();//���ߺ���: ����������������� 7 ����ռλ��nullBlock����, ���û�п��������, �򷵻�false
	bool absorb();//���պ���: ��ͼ�����տ��Ժϳ� 7 �Ŀ�, ������ռλ��nullBlock���汻������,�޸�������Ϊ 7 ,���û�з��������򷵻� false
public:

	//�Զ��� �� ��, ���ڼ�¼ ���ߺ��� �� ���պ��� �еĿ��Ա������ һ��NumBlock ������ʵ�ʵļ����޸�
	struct group {
		vector<NumBlock>::iterator center;//���Ŀ�,��ʾ ���պ��� �������ߺ� ���ߺ��� �е���һ�� 7 ��
		vector<vector<NumBlock>::iterator> neighbor;//�ڽӿ�����,��ʾ ���պ��� �ı������ߺ� ���ߺ��� �ķ����� 7 
		GameMap* map;//��map��ָ��,��clear��ʹ��

		int clear();//���7
		void absorb();//����
		bool operator<(const group&)const;//��������������ʱ��Ҫ�õ�,�����㷨�� ����˵��

		group(vector<NumBlock>::iterator&, vector<vector<NumBlock>::iterator>&, GameMap*);//���캯��
	};

	//����״̬ ö������, ���ڼ�¼��ǰ����Ϸ����״̬,���� �����߼����㺯��״̬ �� ��ʼ ��ֹ,��execut()��ʹ��
	enum Function {
		ABSORB, 
		TRIM, 
		CLEAR, 
		BEGIN, 
		END
	};

	GameMap();
	int getScore()const;
	bool isOutOfRange();//��Ϸ�����ж�,�Ƿ�mapԽ��
	void setAllUnActive();//�趨map��ȫ��Ϊ�ǻ״̬
	void insert(NumBlock&, unsigned int);//���� �����BlockPair ����map�ļ���
	Function execute(Function);//���ⲿ�Ľӿ�,�ⲿͨ����һ���������в��ϵص��ú�״̬�ĸı�ʵ�ּ���,�����㷨�� ����˵��
	void print(ostream& out = cout);

	inline vector<NumBlock>::iterator index(unsigned int, unsigned int);//��ĳһ�������������ת��


	//��������������ģ�庯��,������������,��Ҫ�����Ƕ�ĳһ�������Χ�ĸ�����б���,�ж�����ΪisValid,ִ�в���Ϊconduct
	//������ܱߵ��ĸ�������isValid������ִ��conduct����
	template<typename Judge, typename Body>
	void beside(unsigned int col, unsigned int row, Judge isValid, Body conduct);

	template<typename Judge, typename Body>
	void beside(vector<NumBlock>::iterator, Judge isValid, Body conduct);
};


inline vector<NumBlock>::iterator GameMap::index(unsigned int col, unsigned int row)
{
	return ((map.begin() + col)->begin()) + row;
}

//����������ָ�����ıȽ�
bool operator==(const vector<NumBlock>::iterator& liter, const vector<NumBlock>::iterator& riter);

//�ⲿʹ��, ʹBlockPair����GameMap
void fall(BlockPair& bp, GameMap& gmp);

//��ӡ��Ϸ����
void print(ostream& out, BlockPair& bp, GameMap& gmp);
