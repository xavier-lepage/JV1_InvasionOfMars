#include "ScoreTag.h"
#include "ContentPipeline.h"

std::stack<ScoreTag*> ScoreTag::scoreTagStack;

ScoreTag::ScoreTag()
	: scoreText(new Text(ContentPipeline::getInstance().getFont(), "", 16U))
{
	this->scoreText->setFillColor(Color::White);
}

ScoreTag::~ScoreTag()
{
	if (this->scoreText != nullptr) delete this->scoreText;
}

void ScoreTag::update(const float deltaTime)
{
	if (this->despawnTimer > 0.0f) this->despawnTimer -= deltaTime;
	else this->deactivate();
}

void ScoreTag::deactivate()
{
	GameObject::deactivate();
	this->addToScoreTagStack();
}

void ScoreTag::draw(RenderWindow& renderWindow) const
{
	if (this->isActive()) renderWindow.draw(*this->scoreText);
}

void ScoreTag::spawn(const Vector2f& spawnLocation, const unsigned int score)
{
	ScoreTag* scoreTag = getAvailableScoreTag();

	if (scoreTag == nullptr) return;

	scoreTag->scoreText->setPosition(spawnLocation);
	scoreTag->scoreText->setString(to_string(score));
	scoreTag->despawnTimer = SCORE_TAG_DURATION;
	scoreTag->activate();
}

ScoreTag* ScoreTag::getAvailableScoreTag()
{
	if (scoreTagStack.empty()) return nullptr;

	ScoreTag* scoreTag = scoreTagStack.top();
	scoreTagStack.pop();
	return scoreTag;
}

void ScoreTag::addToScoreTagStack()
{
	scoreTagStack.push(this);
}
