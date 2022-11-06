#pragma once
#include "TYSI\SIBaseModel.h"
#define 说 ->say
#define 结束 storyEnd()
#define 分支选项 branch
#define 如果 if (branchIn(
#define 则 ))

class Part1 :public StoryManager
{
public:
	Character *雅音;
	Part1(): StoryManager("Part1", "Part1") {
		Player = new Character(this, "Player");
		雅音 = new Character(this, "雅音");
		this->doSpawn();
	}
	void defStory() {
		雅音 说("接下来你打算怎么处理这破事", false);
		分支选项({ "先看看咋回去","接着干再说","我不知道"});
		如果 "先看看咋回去" 则{
			雅音 说("有一说一这事确实难整");
			雅音 说("不过我会尽力");
			雅音 说("你先去睡觉吧");
		}
		如果 "接着干再说" 则{
			雅音 说("其实我不太推荐");
			雅音 说("你还是先休息吧");
		}
		如果 "我不知道" 则{
			雅音 说("....emmmmm");
		}
		雅音 说("唉");
		结束;
	}
};