package net.jllama.api

import net.jllama.core.GgmlType
import net.jllama.core.LlamaContext
import net.jllama.core.LlamaBatch
import net.jllama.core.LlamaContextParams
import spock.lang.Shared
import spock.lang.Specification
import spock.lang.Stepwise

import java.util.stream.Collectors
import java.util.stream.IntStream

@Stepwise
class ContextTokenFlowSpec extends Specification {

  LlamaContext mockCoreContext
  @Shared
  Context underTest

  final static int CONTEXT_SIZE = 1000
  final static int SEQUENCE_1_LENGTH = 20
  final static int SEQUENCE_2_LENGTH = 40

  def setupSpec() {
    final coreContextParams = new LlamaContextParams()
    coreContextParams.setnCtx(CONTEXT_SIZE)
    coreContextParams.setnBatch 500
    coreContextParams.setnThreads 1
    coreContextParams.setnThreadsBatch 1
    coreContextParams.setRopeScalingType(-1 as byte)
    coreContextParams.setYarnExtFactor(-1.0f)
    coreContextParams.setYarnAttnFactor(1.0)
    coreContextParams.setYarnBetaFast(32.0)
    coreContextParams.setYarnBetaSlow(1.0)
    coreContextParams.setTypeK(GgmlType.GGML_TYPE_F16)
    coreContextParams.setTypeV(GgmlType.GGML_TYPE_F16)
    coreContextParams.setOffloadKqv(true)
    underTest = new Context(Mock(LlamaContext), coreContextParams)
  }

  def setup() {
    // see https://github.com/spockframework/spock/issues/791
    mockCoreContext = Mock()
    underTest.llamaContext = mockCoreContext
  }

  def "Sequences are submitted to batch"() {
    when:
    final Batch batch = underTest.batch().type(Context.SequenceType.TOKEN).get()
    final sequencePiece1 = Sequence.tokenSequence(1)
        .piece(mockTokenList(SEQUENCE_1_LENGTH))
    final sequencePiece2 = Sequence.tokenSequence(2)
        .piece(mockTokenList(SEQUENCE_2_LENGTH))
    batch.stage(sequencePiece1)
    batch.stage(sequencePiece2)

    then:
    1 * mockCoreContext.llamaBatchInit(_, _, _) >> {
      final int nTokens = it[0] as int
      final int nSeqMax = it[2] as int
      return new LlamaBatch(0l, 0, new int[nTokens], new float[0],
          new int[nTokens], new int[nTokens], new int[nTokens][nSeqMax], new byte[nTokens])
    }
    batch.stagedSequences.size() == 2
    batch.stagedSequences.get(0) == sequencePiece1
    batch.stagedSequences.get(1) == sequencePiece2
    underTest.tokensBatch == batch
    underTest.embeddingsBatch == null
  }

//  def "Context evaluates batch"() {
//    when: "The batch is evaluated"
//    underTest.evaluate(underTest.batch()
//        .type(Context.SequenceType.TOKEN)
//        .get())
//
//    then: "A core LlamaBatch is passed to the decode() method"
//    1 * mockCoreContext.llamaDecode(_ as LlamaBatch) >> {
//      final LlamaBatch batch = it[0] as LlamaBatch
//      batch.nTokens == SEQUENCE_1_LENGTH + SEQUENCE_2_LENGTH
//      return 0
//    }
//  }


  def "clearSequences() - Context completely cleared"() {

  }

  private static List<Integer> mockTokenList(final int length) {
    return IntStream.range(0, length)
        .map { 0 }
        .boxed()
        .collect(Collectors.toList())
  }
}