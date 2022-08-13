# Extractor
Library which helps you extract variables from text.

# Help
The Extractor is currently in need of Extract type (variables which can be checked in extractor) and Modifier (addon patterns which augment Extractor's abilities) implemetentions.

* check instruction.inl for Extract types and small Modifiers
* check modifier.inl for complex Modifiers
* check core.hpp and extractor.hpp for better insight
* check test/testmodifiers.cpp for small show

# Goal
The main object of Extractor library is to help you find and extract specific variables from a text using as many templates and metaprogramming as possible to make your release version faster. However it also contains bindings to runtime.

