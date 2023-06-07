struct level_0 {
	virtual ~level_0() {};
	virtual int compute() { return 0; }
};

struct level_1a : level_0 {
	virtual int compute() = 0;
};

struct level_1b : level_0 {
	virtual int recompute() = 0;
};

struct level_2e : level_1a {
	int recompute(int) { return 3; }
};

struct level_2f : level_1b {
	int recompute(int) { return 4; }
};

int main()
{
    // instantiate objects of all classes
    level_0* l0 = new level_0();
    level_1a* l1a = new level_1a();
    level_1b* l1b = new level_1b();
    level_2e* l2e = new level_2e();
    
}