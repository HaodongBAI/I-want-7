#pragma once
#include <iostream>
#include <vector>
#include <array>
using namespace std;

//游戏中的 块 类
class NumBlock {
private:
	unsigned int number;//块的数字
	bool active;//块是否活跃,即上一步是否发生改变,在GameMap::absorb()中使用
	unsigned int lock;//块的枷锁情况,如果数字是7则表示加星情况
	NumBlock& setlock(unsigned int);
public:
	NumBlock();//随机数字构造块
	NumBlock(int, bool, unsigned int);
	NumBlock(const NumBlock&);//拷贝构造
	NumBlock& operator=(const NumBlock&);
	bool operator==(const NumBlock&)const;

	bool isActive()const;
	unsigned int getNumber()const;
	unsigned int getLock()const;

	NumBlock& setActive(bool);
	NumBlock& unlock();//解锁,当一定规则下可以对其进行解锁,lock减一
	bool isAbsorbable(const NumBlock&)const;//是否可对 参数块 进行吸收

	friend ostream& operator<<(ostream&, const NumBlock&);//输出块
};

//静态的 空块 对象,用于游戏过程中填充位置
static const NumBlock nullBlock(8, false, 0);


//游戏中的 下落块 类,下落块由两个NumBlock组成,还记录了两者关系
class BlockPair {
public:
	//自定义的枚举类型,记录第二个块相对于第一个块的位置
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

	BlockPair& turn();//转动 下落块 的方向
	void print(ostream&)const;
	void move(char);//移动 下落块 在map中的位置
private:
	array<NumBlock, 2> pair;
	Direction direction;
	unsigned int firstloc;//记录第一个NumBlock的位置
	unsigned int secondloc;//记录第二个NumBlock的位置
	void _init();//初始化,得到两个可以共存的NumBlock
};

//游戏中的 地图 类,主体是一个二维数组
class GameMap {
private:
	array<vector<NumBlock>, 6> map;//主体地图,共6列和最长8行,采用array<,6>记录列,vector记录行
	int score;//记录分数
private:
	//内部函数,负责主要的逻辑计算,由execut()函数调用
	bool trim();//剪切函数: 清除 空块(nullBlock,占位符), 如果没有可以清除的 块 则返回 false
	bool clearSeven();//消七函数: 清除至少三个相连的 7 并用占位符nullBlock代替, 如果没有可以清除的, 则返回false
	bool absorb();//吸收函数: 在图中吸收可以合成 7 的块, 并且用占位符nullBlock代替被吸收者,修改吸收者为 7 ,如果没有发生吸收则返回 false
public:

	//自定义 组 类, 用于记录 消七函数 和 吸收函数 中的可以被处理的 一组NumBlock 并进行实际的计算修改
	struct group {
		vector<NumBlock>::iterator center;//中心块,表示 吸收函数 的吸收者和 消七函数 中的任一个 7 块
		vector<vector<NumBlock>::iterator> neighbor;//邻接块数组,表示 吸收函数 的被吸收者和 消七函数 的非中心 7 
		GameMap* map;//对map的指针,在clear中使用

		int clear();//清除7
		void absorb();//吸收
		bool operator<(const group&)const;//对吸收序列排序时需要用到,具体算法见 程序说明

		group(vector<NumBlock>::iterator&, vector<vector<NumBlock>::iterator>&, GameMap*);//构造函数
	};

	//函数状态 枚举类型, 用于记录当前的游戏运行状态,包括 三大逻辑运算函数状态 和 开始 终止,在execut()中使用
	enum Function {
		ABSORB, 
		TRIM, 
		CLEAR, 
		BEGIN, 
		END
	};

	GameMap();
	int getScore()const;
	bool isOutOfRange();//游戏结束判断,是否map越界
	void setAllUnActive();//设定map中全部为非活动状态
	void insert(NumBlock&, unsigned int);//用于 下落块BlockPair 进入map的计算
	Function execute(Function);//对外部的接口,外部通过这一个函数进行不断地调用和状态的改变实现计算,具体算法见 程序说明
	void print(ostream& out = cout);

	inline vector<NumBlock>::iterator index(unsigned int, unsigned int);//对某一坐标向迭代器的转换


	//以下两个函数是模板函数,并进行了重载,主要作用是对某一个块的周围四个块进行遍历,判断条件为isValid,执行操作为conduct
	//如果其周边的四个块满足isValid条件则执行conduct操作
	template<typename Judge, typename Body>
	void beside(unsigned int col, unsigned int row, Judge isValid, Body conduct);

	template<typename Judge, typename Body>
	void beside(vector<NumBlock>::iterator, Judge isValid, Body conduct);
};


inline vector<NumBlock>::iterator GameMap::index(unsigned int col, unsigned int row)
{
	return ((map.begin() + col)->begin()) + row;
}

//两个迭代器指向对象的比较
bool operator==(const vector<NumBlock>::iterator& liter, const vector<NumBlock>::iterator& riter);

//外部使用, 使BlockPair进入GameMap
void fall(BlockPair& bp, GameMap& gmp);

//打印游戏界面
void print(ostream& out, BlockPair& bp, GameMap& gmp);
