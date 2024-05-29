#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Candidate {
private:
    string name;
    string symbolPath;
    string motto;
    int votes;
    RectangleShape voteButton;
    Text voteButtonText;
    Text votesText;
    Color mottoColor;

public:
    Candidate(const string& name, const string& symbolPath, const string& motto, Color mottoColor)
        : name(name), symbolPath(symbolPath), motto(motto), votes(0), mottoColor(mottoColor) {
        voteButton.setSize(Vector2f(100, 30));
        voteButton.setFillColor(Color::Green);
    }

    const string& getName() const { return name; }
    const string& getSymbolPath() const { return symbolPath; }
    const string& getMotto() const { return motto; }
    int getVotes() const { return votes; }

    void addVote() { ++votes; updateVotesText(); }
    void removeVote() { if (votes > 0) --votes; updateVotesText(); }

    void setVoteButtonPosition(float xPos, float yPos) {
        voteButton.setPosition(xPos, yPos);
    }

    const RectangleShape& getVoteButton() const {
        return voteButton;
    }

    void setVoteButtonText(const Font& font) {
        voteButtonText.setFont(font);
        voteButtonText.setString("Vote Now");
        voteButtonText.setCharacterSize(16);
        voteButtonText.setFillColor(Color::White);
        voteButtonText.setPosition(
            voteButton.getPosition().x + (voteButton.getSize().x - voteButtonText.getLocalBounds().width) / 2,
            voteButton.getPosition().y + (voteButton.getSize().y - voteButtonText.getLocalBounds().height) / 2 - 5
        );
    }

    const Text& getVoteButtonText() const {
        return voteButtonText;
    }

    void updateVotesText() {
        votesText.setString("Votes: " + to_string(votes));
    }

    void setVotesTextPosition(float xPos, float yPos, const Font& font) {
        votesText.setFont(font);
        votesText.setCharacterSize(18);
        votesText.setFillColor(Color::Black);
        votesText.setPosition(xPos, yPos);
        updateVotesText();
    }

    const Text& getVotesText() const {
        return votesText;
    }

    void display(RenderWindow& window, float xPos, float yPos, Font& font) const {
        // Display candidate name and motto
        Text nameText;
        nameText.setFont(font);
        nameText.setString(name);
        nameText.setCharacterSize(24);
        nameText.setFillColor(Color::Black);
        nameText.setPosition(xPos, yPos);

        Text mottoText;
        mottoText.setFont(font);
        mottoText.setString(motto);
        mottoText.setCharacterSize(18);
        mottoText.setFillColor(mottoColor);
        mottoText.setPosition(xPos, yPos + 30);

        // Display candidate symbol picture
        Texture symbolTexture;
        if (symbolTexture.loadFromFile(symbolPath)) {
            Sprite symbolSprite(symbolTexture);
            symbolSprite.setScale(0.5f, 0.5f); // Adjust the scale as needed
            symbolSprite.setPosition(xPos, yPos + 70);

            // Draw everything on the window
            window.draw(nameText);
            window.draw(mottoText);
            window.draw(symbolSprite);
        }
        else {
            cout << "Error loading image: " << symbolPath << endl;
        }
    }
};

void displayCandidates(RenderWindow& window, vector<Candidate>& candidates, Font& font) {
    // Configure the position for the first candidate
    float startX = 50;
    float startY = 50;

    // Configure the spacing between candidates
    float xOffset = 350;
    float yOffset = 300;

    for (size_t i = 0; i < candidates.size(); ++i) {
        // Calculate the position for each candidate based on the row and column
        float xPos = startX + (i % 2) * xOffset;
        float yPos = startY + (i / 2) * yOffset;

        candidates[i].display(window, xPos, yPos, font); // Call display method of Candidate

        // Set and draw the vote button
        candidates[i].setVoteButtonPosition(xPos, yPos + 200);
        candidates[i].setVoteButtonText(font);
        window.draw(candidates[i].getVoteButton());
        window.draw(candidates[i].getVoteButtonText());

        // Set and draw the votes text
        candidates[i].setVotesTextPosition(xPos + 150, yPos + 200, font);
        window.draw(candidates[i].getVotesText());
    }
}

void displayWinner(RenderWindow& window, const string& winnerName, Font& font) {
    Texture winnerTexture;
    if (winnerTexture.loadFromFile("D:\\VS Projects\\Online Voting System\\images\\Public-Opinion_1400x840-ezgif.com-webp-to-jpg-converter.jpg")) {
        Sprite winnerSprite(winnerTexture);
        window.draw(winnerSprite);
    }
    else {
        cout << "Error loading winner image!" << endl;
    }

    RectangleShape congratsRect(Vector2f(600, 100));
    congratsRect.setFillColor(Color::Black);
    congratsRect.setPosition(400, 350);

    Text congratsText;
    congratsText.setFont(font);
    congratsText.setString("Congratulations " + winnerName + " for winning!");
    congratsText.setCharacterSize(30);
    congratsText.setFillColor(Color::White);
    congratsText.setPosition(420, 380);

    window.draw(congratsRect);
    window.draw(congratsText);
}

void displayPledges(RenderWindow& window, Font& font, RectangleShape& goBackButton, Text& goBackButtonText) {
    Texture pledgeTexture;
    if (!pledgeTexture.loadFromFile("D:\\VS Projects\\Online Voting System\\images\\pledge.jpg")) {
        cout << "Error loading pledge image!" << endl;
        return;
    }
    Sprite pledgeSprite(pledgeTexture);
    window.draw(pledgeSprite);

    window.draw(goBackButton);
    window.draw(goBackButtonText);
}

void castVote(vector<Candidate>& candidates, int candidateIndex, stack<int>& voteStack) {
    if (candidateIndex >= 0 && candidateIndex < candidates.size()) {
        candidates[candidateIndex].addVote();
        voteStack.push(candidateIndex);
        cout << "Thank you for voting for " << candidates[candidateIndex].getName() << "!\n";
    }
    else {
        cout << "Invalid candidate selection.\n";
    }
}

void undoLastVote(vector<Candidate>& candidates, stack<int>& voteStack) {
    if (!voteStack.empty()) {
        int lastVoteIndex = voteStack.top();
        voteStack.pop();
        candidates[lastVoteIndex].removeVote();
        cout << "Last vote undone for " << candidates[lastVoteIndex].getName() << ".\n";
    }
    else {
        cout << "No votes to undo.\n";
    }
}

int main() {
    RenderWindow window(VideoMode(1400, 900), "Voting System");

    vector<Candidate> candidates = {
        Candidate("Bashir Imran", "D:\\VS Projects\\Online Voting System\\images\\1000_F_561484597_TWm6y6OGLjhqsaxXd62C9j4PyakjfXsh.jpg", "A cap for every head", Color::Blue),
        Candidate("Hiba ", "D:\\VS Projects\\Online Voting System\\images\\turban-headdress-isolated-on-white-background-vector-37490856.jpg", "Unity in diversity", Color::Magenta),
        Candidate("Nimra & Hafsa", "D:\\VS Projects\\Online Voting System\\images\\9713c63db4286370ab5e8fa5e2e59a31.jpg", "A rose for a better future", Color::Red),
        Candidate("Hafiz", "D:\\VS Projects\\Online Voting System\\images\\imp.jpg", "Embrace change", Color::Red),
        Candidate("Hassnain Abbas", "D:\\VS Projects\\Online Voting System\\images\\01f49cc6e618322fce0c7656caa7ee07.jpg","Strength and honor", Color::Magenta)
    };

    Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        cout << "Error loading font!" << endl;
        return -1;
    }

    // Title screen setup
    Texture titleTexture;
    if (!titleTexture.loadFromFile("D:\\VS Projects\\Online Voting System\\images\\elections.jpg")) {
        cout << "Error loading title image!" << endl;
        return -1;
    }
    Sprite titleSprite(titleTexture);

    // Vote Now button setup
    RectangleShape voteNowButton(Vector2f(200, 50));
    voteNowButton.setFillColor(Color::Green);
    voteNowButton.setPosition(600, 750);

    Text voteNowButtonText;
    voteNowButtonText.setFont(font);
    voteNowButtonText.setString("Vote Now");
    voteNowButtonText.setCharacterSize(20);
    voteNowButtonText.setFillColor(Color::Black);
    voteNowButtonText.setPosition(
        voteNowButton.getPosition().x + (voteNowButton.getSize().x - voteNowButtonText.getLocalBounds().width) / 2,
        voteNowButton.getPosition().y + (voteNowButton.getSize().y - voteNowButtonText.getLocalBounds().height) / 2 - 5
    );

    // End elections button
    RectangleShape endElectionsButton(Vector2f(200, 50));
    endElectionsButton.setFillColor(Color::Red);
    endElectionsButton.setPosition(700, 20);

    Text endElectionsButtonText;
    endElectionsButtonText.setFont(font);
    endElectionsButtonText.setString("End Elections");
    endElectionsButtonText.setCharacterSize(20);
    endElectionsButtonText.setFillColor(Color::White);
    endElectionsButtonText.setPosition(
        endElectionsButton.getPosition().x + (endElectionsButton.getSize().x - endElectionsButtonText.getLocalBounds().width) / 2,
        endElectionsButton.getPosition().y + (endElectionsButton.getSize().y - endElectionsButtonText.getLocalBounds().height) / 2 - 5
    );

    // Pledges button setup
    RectangleShape pledgesButton(Vector2f(300, 50));
    pledgesButton.setFillColor(Color::Yellow);
    pledgesButton.setPosition(500, 700);

    Text pledgesButtonText;
    pledgesButtonText.setFont(font);
    pledgesButtonText.setString("Candidates' Pledges/Promises");
    pledgesButtonText.setCharacterSize(20);
    pledgesButtonText.setFillColor(Color::Black);
    pledgesButtonText.setPosition(
        pledgesButton.getPosition().x + (pledgesButton.getSize().x - pledgesButtonText.getLocalBounds().width) / 2,
        pledgesButton.getPosition().y + (pledgesButton.getSize().y - pledgesButtonText.getLocalBounds().height) / 2 - 5
    );

    // Go Back button setup (outside of displayPledges)
    RectangleShape goBackButton(Vector2f(200, 50));
    goBackButton.setFillColor(Color::Blue);
    goBackButton.setPosition(600, 750);

    Text goBackButtonText;
    goBackButtonText.setFont(font);
    goBackButtonText.setString("Go Back");
    goBackButtonText.setCharacterSize(20);
    goBackButtonText.setFillColor(Color::White);
    goBackButtonText.setPosition(
        goBackButton.getPosition().x + (goBackButton.getSize().x - goBackButtonText.getLocalBounds().width) / 2,
        goBackButton.getPosition().y + (goBackButton.getSize().y - goBackButtonText.getLocalBounds().height) / 2 - 5
    );

    // Undo Last Vote button setup
    RectangleShape undoVoteButton(Vector2f(200, 50));
    undoVoteButton.setFillColor(Color::Blue);
    undoVoteButton.setPosition(500, 20);

    Text undoVoteButtonText;
    undoVoteButtonText.setFont(font);
    undoVoteButtonText.setString("Undo Last Vote");
    undoVoteButtonText.setCharacterSize(20);
    undoVoteButtonText.setFillColor(Color::White);
    undoVoteButtonText.setPosition(
        undoVoteButton.getPosition().x + (undoVoteButton.getSize().x - undoVoteButtonText.getLocalBounds().width) / 2,
        undoVoteButton.getPosition().y + (undoVoteButton.getSize().y - undoVoteButtonText.getLocalBounds().height) / 2 - 5
    );

    stack<int> voteStack; // Stack to track the votes
    bool electionsEnded = false;
    bool showTitleScreen = true;
    bool showPledges = false; // Track if the pledges screen should be shown
    string winnerName;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (showTitleScreen) {
                        if (voteNowButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            showTitleScreen = false;
                        }
                        else if (pledgesButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            showPledges = true;
                            showTitleScreen = false;
                        }
                    }
                    else if (showPledges) {
                        if (goBackButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            showPledges = false;
                            showTitleScreen = true;
                        }
                    }
                    else {
                        if (endElectionsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            // End elections and find the winner
                            electionsEnded = true;
                            int maxVotes = 0;
                            for (const auto& candidate : candidates) {
                                if (candidate.getVotes() > maxVotes) {
                                    maxVotes = candidate.getVotes();
                                    winnerName = candidate.getName();
                                }
                            }
                        }
                        if (undoVoteButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            undoLastVote(candidates, voteStack);
                        }
                        for (size_t i = 0; i < candidates.size(); ++i) {
                            if (candidates[i].getVoteButton().getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                castVote(candidates, i, voteStack);
                            }
                        }
                    }
                }
            }
        }

        window.clear(Color::White);

        if (showTitleScreen) {
            window.draw(titleSprite);
            window.draw(voteNowButton);
            window.draw(voteNowButtonText);
            window.draw(pledgesButton);
            window.draw(pledgesButtonText);
        }
        else if (electionsEnded) {
            displayWinner(window, winnerName, font);
        }
        else if (showPledges) {
            displayPledges(window, font, goBackButton, goBackButtonText);
        }
        else {
            window.draw(endElectionsButton);
            window.draw(endElectionsButtonText);
            window.draw(undoVoteButton);
            window.draw(undoVoteButtonText);
            displayCandidates(window, candidates, font);
        }

        window.display();
    }

    return 0;
}