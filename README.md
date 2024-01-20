# jllama
A WIP library that provides Java bindings for [llama.cpp](https://github.com/ggerganov/llama.cpp).

The project is split into two parts, a fluent API for managing Llama models and contexts, and a lower level API that provides direct access to the "public" interface of llama.cpp

## API
The root of the API is the `net.jllama.api.Llama` class. All access to the LlaMA flows from here.

### Accessing the API
```java
Llama.library()
```

## The Model
A Model represents a trained LlaMA model. It can be used to create contexts, which are used to generate tokens. Any number of models can be loaded at once, bounded only by the amount of memory available on the machine. JVM heap space is not used for this allocation.

### Loading a Model

```java
Model model = Llama.library()
  .newModel()
  .with() 
  .path("/path/to/model.gguf")
  .load();
```


## The Context
A Context represents a single working instance of a Model. A context is stateful, containing the current relationships between all available tokens. These relationships are defined through "sequences." A context can contain one or more sequences of tokens at a given time.

### Creating a Context

```java
Context context = mode.newContext()
  .withDefaults() // use with() if you don't want to prepopulate with default values
  .cotextLength(1000)
  .seed(ThreadLocalRandom.current().nextInt())
  .create();
```

## Tokens
A token represents a word-piece, a piece of the model's vocabulary. Llama operates on tokens using contexts, so text must be converted to and from tokens when interacting with the context.

The model provides access to special tokens and to tokenization functionality.

### Tokenizing Text

```java
List<Integer> tokens = model.tokens().tokenize(text);
```

### Detokenizing Text

```java
String text = model.tokens().detokenize(tokens);
```
### Evaluation
Sequences are added to a context with the evaluation of a "batch" of tokens. Evaluation adds the batched sequences (or sequence updates) and internally calculates the probabilities for the next token.

## Adding a sequence to a Context

```java
int seqId = 0; // NOTE: id must be unique per sequence
Sequence sequence = Sequence.tokenSequence(seqId);
Batch batch = context.batch()
  .type(SequenceType.TOKEN)
  .get()
  .stage();
batch.stage(sequence.piece(tokens));
context.evaluate(batch);
```

Evaluation internally calculates probabilities, known as "logits," for the next token in the sequence. These logits are the probability for each token in the vocabulary to be the next token in the sequence.

### Getting logits after evaluation
```java
List<Float> logits = context.getLogits(sequence);
```

## Sampling
After logits are calculated, a token must be picked from the probabilities. The context provides a sampler to optionally apply sampling criteria before sampling (picking) the next token.

### Sampling a Token
```java
int token = context.sampler(logits)
  .keepTopK(50)
  .applyTemperature(1.1f)
  .sample();
```
This token can then be translated back to readable text by invoking `model.tokens().detokenize(token)`.

## Examples
For examples of use, including a simple prompt evaluator and a Spring-based Chatbot, see [jllama-examples](https://github.com/crimsonmagick/jllama-examples)

## Logging 
Logging is provided by log4j2-api, which can be integrated with the logger implementation of your choice.

## Core
A core library is also provided, which endeavors provides a close to one-to-one mapping to llama.cpp's C interface.

More documentation will be available soon.