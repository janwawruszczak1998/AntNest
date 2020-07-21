### How to run program (Ubuntu 18.04)

Install necessary packages (cmake to build and ncurses to display it in your console)

```bash
sudo apt-get -y install libncurses5-dev
sudo apt-get -y install cmake
```

Clone repository, build and make

```bash
git clone https://github.com/janwawruszczak1998/AntNest.git
cd AntNest
cmake ./
make
./AntNest
```

### What is that?

It is simple multithreading program, where we can observe 2 antnests. Small ants-workers collect pieces of food and bring them to nests. Nest entrance is synchronized (with conditional variable), what means at most one ant can use it in one time unit. What is more, nest procuces eggs if posses some food and babysit new ants if ants-babysitters egsist. Bigger ants-warriors follow insect outside nest, but need to be hurry, because warriors from opposite nest follow insect too - its shared resource.

### Why is that?

I wanted to learn something about multithreading in C++. And I like ants :)

Possible view in VSCode, but ants run there, trust me!

![image of simulation frame](https://github.com/janwawruszczak1998/AntNest/blob/master/ants_running.png)
