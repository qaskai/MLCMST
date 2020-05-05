#pragma once

template <typename Params>
class App
{
public:
    virtual ~App();
    void start(int argc, char** argv);

protected:
    virtual cxxopts::Options createOptions() = 0;
    virtual void validateParseResult(const cxxopts::ParseResult& result) = 0;
    virtual Params extractParams(const cxxopts::ParseResult& result) = 0;
    virtual void run(const Params& params) = 0;
};

template <typename Params>
App<Params>::~App() = default;

template <typename Params>
void App<Params>::start(int argc, char** argv)
{
    cxxopts::Options options = createOptions();
    cxxopts::ParseResult result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return;
    }
    validateParseResult(result);
    Params params = extractParams(result);
    run(params);
}
