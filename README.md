# TianyuStory (Qt Version) NotQuests剧情脚本转换器

众所周知嗷，YAML文件是没有逻辑的，而NotQuests、Quests等插件为了搞出来复杂的剧情逻辑，在YAML里面用TAG满天飞的方式解决了问题。

诚然，在汇编的时代，各种跳转满天飞是常态，但是对于现在的编程习惯而言确实没有办法接受。

所以本项目解决的就是这样一个问题，如何从现代编程语言转换到NotQuests的YAML剧情脚本。

**啥玩意是自解释，本项目中的意思是，这个代码本身运行的时候会变成YAML剧情脚本，也就是把程序运行时作为自己解释自己的手段**

# 用法(适用于原版TianyuStory也就是Py版)

* 1.导入文件
  ```C++
  include "TYSI/SIBaseModel.h"
  ``` 
  或 
  ```Python
  from TYSI.SIBaseModel import *
  ```
  
* 2.继承`StoryManager`基类
  ```C++
  class YourStory :public StoryManager
  ```
  或
  ```Python
  class YourStory(StoryManager)
  ```
 
* 3.编写构造函数

  在构造函数里面你要做三个事情：调用基类构造函数以确定生成目标地址，建立基本人物对象或做其他事情，调用doSpawn()
  * 调用基类构造函数
    ```C++
    YourStory():StoryManager("FilePath","FileName"){
      
    }
    ```
    或
    ```Python
    def __init__(this):
      super().__init__(__name__, this.__class__.__name__)
    ```
    Python上再单独解释一下，这里采用这种方式可以自动获取Python模块的相关信息，最后可以使生成的YML文件和Python文件维持相同的文件结构，对于自动管理多个故事的结构十分有效。
  * 建立基本人物对象
    ```C++
    Player = new Character(this, "Player");
    雅音 = new Character(this, "雅音");
    ```
    此处Player严格按照此写法进行，不要改变。此处是由于设计问题只能出现在这里由用户初始化，Python则不要自己初始化Player，例如：
    ```Python
    雅音 = Character(this, "雅音")
    ```
  * 调用doSpawn()
    ```C++
    this->doSpawn();
    ```
    或
    ```Python
    this.doSpawn()
    ```
* 4.开始搞事情
    现在，正式开始写剧情只需要重写函数defStory()，即
    ```C++
    void defStory(void){
      ...
    }
    ```
    或
    ```Python
    def defStory(this):
      ...
    ```

**通过以上例子，我们可以发现Python和C++版的TianyuStory差别并不大，所以下面不再单独列出C++教程，仅以Python语言示范**

# 写剧情（搞事情）

按照我的习惯，我们直接讲例子，毕竟一个200行不到的自解释器你指望有多少特性
```Python
def defStory(this):
        this.雅音.say("那小子又给你说啥了？")
        this.雅音.say("先别管他了")
        this.雅音.say("接下来你打算去哪", False)
        this.branch(["去雪原镇","去星见镇"])
        if this.branchIn(0):
            this.雅音.say("那你就去吧，我这就带你去。", False)
            this.branch(["好的","不用了"])
            if this.branchIn(0):
                this.雅音.say("那我们就走吧。")
                this.storyEnd()
            if this.branchIn(1):
                this.雅音.say("那就后会有期了")
                this.storyEnd()
            this.branchEnd()
        if this.branchIn(1):
            this.雅音.say("那我们就去星见镇吧。", False)
        this.branchEnd()
        this.Player.say("好的。")
        this.storyEnd()
 ```
 * 特性1：调用Character.say函数，自动生成Player的等待文本，默认值是“Next > >”
 * 特性2：调用Character.say函数，用False作为第二参数，可以取消自动生成Player等待文本
   * 注意：只要想自定义Player回复文本，就应当取消上一个Player等待文本的自动生成 
 * 特性3：调用this.branch函数，传入一个列表，可以初始化一个剧情分支
   * 注意：在this.branch初始化剧情分支之前，如果有一个NPC在讲话，应该传入False取消他的Player等待自动生成，否则等待文本会和选项文本冲突
 * 特性4：在this.branch后，调用if this.branchIn(int)即可进入分支选项的编写
   * 注意：分支可以嵌套，但不稳定
   * 注意：只要你看了源码你就会发现这玩意每次都返回True，这就是说，我们利用了Python和C++的语法缩进和括号范围来提供更加直观的分支编写，所以这if对于编写者本身是有意义的，但是对于Python或者C++语言本身意义不大，调用此函数后。后台会切换当前的分支标记以供if内的剧情文本获取，从而实现YAML的标签跳转逻辑。
   * 注意：生怕有人读完上面一条还没有想到：由于上条阐述的代码机理，不要写else或elif，只能写if
   * 注意：branchIn理论上对应分支序号，但是实际上该参数没有意义，每次调用this.branchIn后内部计数自动+1，所以如果需要更高可读性，也可直接在branchIn()里面写上对应的分支文本，Python通过Any接受任意参数，C++有一个QString重载可以接受字符串参数。
   * 注意：每个branch结束时，应当调用this.branchEnd()进行数据回收
 * 特性5：需要中途终止或在分之内终止故事时，调用this.storyEnd()
   * 注意：这个storyEnd不想与break或者return之类的玩意，因此在storyEnd之后，该需要的branchEnd一个不能漏
    
