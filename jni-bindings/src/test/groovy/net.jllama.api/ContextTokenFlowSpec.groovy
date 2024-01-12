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

import static net.jllama.api.Sequence.*

@Stepwise
class ContextTokenFlowSpec extends Specification {

  LlamaContext mockCoreContext
  @Shared
  Context underTest

  final static int CONTEXT_SIZE = 1000
  final static int SEQUENCE_1_ID = 1
  final static int SEQUENCE_2_ID = 2
  final static SequenceId SEQUENCE_ID_1 = new SequenceId(new int[]{SEQUENCE_1_ID})
  final static SequenceId SEQUENCE_ID_2 = new SequenceId(new int[]{SEQUENCE_2_ID})
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
    final sequencePiece1 = tokenSequence(SEQUENCE_1_ID)
        .piece(mockTokenList(SEQUENCE_1_LENGTH))
    final sequencePiece2 = tokenSequence(SEQUENCE_2_ID)
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

  def "Context evaluates batch"() {
    when: "The batch is evaluated"
    underTest.evaluate(underTest.batch()
        .type(Context.SequenceType.TOKEN)
        .get())

    then: "A core LlamaBatch is passed to the decode() method"
    1 * mockCoreContext.llamaDecode(_ as LlamaBatch) >> {
      final LlamaBatch batch = it[0] as LlamaBatch
      batch.nTokens == SEQUENCE_1_LENGTH + SEQUENCE_2_LENGTH
      return 0
    }
    and: "Sequences are added to the Context"
    underTest.sequences.size() == 2
    underTest.sequences.get(SEQUENCE_ID_1).getLength() == SEQUENCE_1_LENGTH
    underTest.sequences.get(SEQUENCE_ID_2).getLength() == SEQUENCE_2_LENGTH
  }

  def "Token positions are corrected when a segment is removed from a sequence"() {
    given:
    final Sequence<? extends Number> sequence = underTest.sequences.get(SEQUENCE_ID_2)
    final int startingLength = sequence.length

    when:
    underTest.removeSequenceSegment(sequence, startPos, endPos)

    then:
    1 * mockCoreContext.llamaKvCacheSeqRm(SEQUENCE_2_ID, startPos, endPos)
    1 * mockCoreContext.llamaKvCacheSeqShift(SEQUENCE_2_ID, endPos, startingLength, expectedDelta)
    sequence.length == startingLength - endPos + startPos

    where:
    startPos | endPos
    0        | 10

    and:
    expectedDelta = -1 * endPos + startPos
  }

  def "Token positions are not shifted when unnecessary"() {
    given:
    final Sequence<? extends Number> sequence = underTest.sequences.get(SEQUENCE_ID_2)
    final int startingLength = sequence.length

    when:
    underTest.removeSequenceSegment(sequence, startPos, startingLength)

    then:
    1 * mockCoreContext.llamaKvCacheSeqRm(2, startPos, startingLength)
    0 * mockCoreContext.llamaKvCacheSeqShift(_, _, _, _)
    sequence.length == startPos

    where:
    startPos = 5
  }


  def "clearSequences() - Context completely cleared"() {
    given:
    final sequence1 = underTest.sequences.get(SEQUENCE_ID_1)
    final sequence2 = underTest.sequences.get(SEQUENCE_ID_2)

    when:
    underTest.clearSequences()

    then:
    underTest.sequences.isEmpty()
    sequence1.length == 0
    sequence2.length == 0
  }

  private static List<Integer> mockTokenList(final int length) {
    return IntStream.range(0, length)
        .map { 0 }
        .boxed()
        .collect(Collectors.toList())
  }
}