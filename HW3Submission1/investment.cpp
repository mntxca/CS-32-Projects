//will not compile (without include statement for <string> and using namespace std, but the code works
class Investment
{
public:
    Investment(string name, int price)
        :m_name(name), m_price(price) {}
    
    virtual ~Investment() {  }; //differs for derived classes, so is virtual
    int purchasePrice()  const { return m_price; } //doesn't need to be virtual as it is same for all
    string name() const { return m_name; } //doesn't need to be virtual as it is same for all
    virtual string description() const { return "investment"; } //differs for derived classes, so is virtual
    virtual bool fungible() const = 0; //differs for derived classes, so is virtual, and Investment might be fungible, so pure virtual

private:
    string m_name;
    int m_price;
    bool m_fungible;
};

class Painting : public Investment
{
public:
    Painting(string name, int price)
        :Investment(name, price) {}
    virtual ~Painting(){ cout << "Destroying " + name() + ", a painting" << endl; }
    virtual string description() const { return "painting"; } //differs for derived classes, so is virtual
    virtual bool fungible() const { return false; }
private:


};

class Stock : public Investment
{
public:
    Stock(string name, int price, string ticker)
        : Investment(name, price), m_ticker(ticker) {}
    virtual ~Stock() { cout << "Destroying " + name() + ", a stock holding" << endl; }
    virtual string description() const { return "stock trading as " + m_ticker; } //differs for derived classes, so is virtual
    virtual bool fungible() const { return true; }
private:
    string m_ticker;
};

class House : public Investment
{
public:
    House(string name, int price)
        :Investment(name, price) {}
    virtual ~House() { cout << "Destroying the house " + name() << endl; }
    virtual string description() const { return "house"; } //differs for derived classes, so is virtual
    virtual bool fungible() const { return false; }
private:
};

