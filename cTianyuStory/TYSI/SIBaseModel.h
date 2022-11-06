#pragma once
#include <QDir>
#include <QtCore>
class Dialog
{
public:
	QString Branch;
	QString Tag;
	QString Text;
	QString Next;
	Dialog(QString tag, QString text, QString branch) {
		Tag = tag;
		Text = text;
		Branch = branch;
		Next = "NONETEXT";
	}
};

class CharacterBase {
public:
	QString Name;
	QString Color;
	QString SOMETHING;
	QList<Dialog> Dialogs;
	virtual void say(QString Text, bool spawnNext = true) = 0;
	virtual void setLastNext(QString) = 0;
};

class StoryManager
{
public:
	friend class CharacterBase;
	friend class Character;
	CharacterBase* LastCharacter;
	CharacterBase* Player;
	QString FirstDialogTag;
	QList<CharacterBase*> CharacterList;
	QString PlayerDefaultText;
	QString StoryName;
	QString StoryPath;
	QStringList Branch = { "Main" };
	QList<int> BranchIndexList = { 0 };
	int CurrentBranchIndex = 0;
	QString CurrentBranch;
	QString LastBranch;

	StoryManager(QString storyPath, QString storyName) {
		QString s = storyPath;
		StoryPath = "TYSIoutput/" + s.replace(".", "\\");
		StoryName = storyName;
		LastCharacter = nullptr;
		PlayerDefaultText = "Next> >";
		FirstDialogTag = "";
		this->branchIn();
		this->LastBranch = this->CurrentBranch;
	}

	void addPlayerNext() {
		this->Player->say(PlayerDefaultText);
	}

	void branch(QStringList branch) {
		CurrentBranchIndex += 1;
		Branch.append(branch.join("::"));
		BranchIndexList.append(0);
		QString LastNext = "";
		for (int i = 0; i < branch.length(); i++) {
			QString Tag = this->Branch.join("_") +"_START" + QString::number(i);
			this->Player->Dialogs.append(Dialog(Tag, branch[i], this->Branch.join("_") + QString::number(i)));
			LastNext += this->Player->Name + "." + Tag + ",";
		}
		LastNext = LastNext.left(LastNext.length() - 1);
		this->LastCharacter->setLastNext(LastNext);
	}
	bool branchIn(QString Any = "") {
		return this->branchIn(0);
	}
	bool branchIn(int Any) {
		int index = this->BranchIndexList.last();
		this->CurrentBranch = this->Branch.join("_") + QString::number(index);
		this->BranchIndexList[BranchIndexList.length() - 1] += 1;
		if (this->LastCharacter != nullptr) { this->LastCharacter = this->Player; }
		return true;
	}
	void branchEnd() {
		this->Branch.removeLast();
		this->BranchIndexList.removeLast();
		int index = this->BranchIndexList.last();
		this->LastBranch = this->CurrentBranch;
		this->CurrentBranch = this->Branch.join("_") + QString::number(index);
	}
	virtual void defStory() = 0;
	void storyEnd() {
		this->LastCharacter->setLastNext("END");
	}
	void doSpawn() {
		this->defStory();
		QStringList SpawnLines;
		SpawnLines.append("start: " + FirstDialogTag + "\n");
		SpawnLines.append("Lines:\n");
		for (auto character = this->CharacterList.begin(); character != this->CharacterList.end(); character++) {
			SpawnLines.append("  " + (* character)->Name + ":\n");
			SpawnLines.append("    color: " + (*character)->Color + "\n");
			for (auto dialog = (*character)->Dialogs.begin(); dialog != (*character)->Dialogs.end(); dialog++) {
				SpawnLines.append("    " + dialog->Tag + ":\n");
				SpawnLines.append("      text: " + dialog->Text + "\n");
				SpawnLines.append("      next: " + dialog->Next + "\n");
				SpawnLines.append("\n");
			}
		}
		QDir dir;
		if (!dir.exists(this->StoryPath)) { dir.mkpath(this->StoryPath); }
		
		QFile SpawnFile;
		SpawnFile.setFileName(this->StoryPath + "\\" + this->StoryName + ".yml");
		if (!SpawnFile.exists()) {
			SpawnFile.open(QIODevice::NewOnly | QIODevice::Text);
		}
		else {
			SpawnFile.open(QIODevice::WriteOnly | QIODevice::Text);
		}
		SpawnFile.write(SpawnLines.join("").toUtf8());
	}
};

class Character : public CharacterBase
{
public:
	StoryManager* Manager;
	Character(StoryManager* manager, QString name) {
		this->Manager = manager;
		Name = name;
		this->Manager->CharacterList.append(this);
		this->Color = "<BLUE>";
	}
	
	 void say(QString Text, bool spawnNext = true) {
		QString Tag = this->Manager->CurrentBranch + "_" + QString::number(this->Dialogs.length());
		if (this->Name != "Player") { Text = "<br><br><br>" + Text + "<br><br><br>"; }
		this->Dialogs.append(Dialog(Tag, Text, this->Manager->CurrentBranch));
		if (this->Manager->LastCharacter != nullptr) {
			this->Manager->LastCharacter->setLastNext(this->Name + "." + Tag);
		}
		else {
			this->Manager->FirstDialogTag = this->Name + "." + Tag;
		}
		this->Manager->LastCharacter = this;
		if (this->Name != "Player" && spawnNext) {
			this->Manager->addPlayerNext();
		}
	}

	void setColor(QString color) {
		this->Color = color;
	}

	void setLastNext(QString next) {
		QString Branch = this->Manager->CurrentBranch;
		for (auto i = this->Dialogs.end(); i != Dialogs.begin();) {
			i--;
			if (i->Branch == Branch) {
				i->Next = next;
				break;
			}
			if (i->Branch.section("_", 0, -1) == this->Manager->LastBranch.section("_", 0, -1) && i->Next == "NONENEXT") {
				i->Next = next;
			}
		}
	}
};

