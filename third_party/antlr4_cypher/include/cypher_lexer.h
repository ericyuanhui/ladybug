
// Generated from Cypher.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  CypherLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    T__38 = 39, T__39 = 40, T__40 = 41, T__41 = 42, T__42 = 43, T__43 = 44, 
    ACYCLIC = 45, ANY = 46, ADD = 47, ALL = 48, ALTER = 49, ANALYZE = 50, 
    AND = 51, AS = 52, ASC = 53, ASCENDING = 54, ATTACH = 55, BEGIN = 56, 
    BY = 57, CALL = 58, CASE = 59, CAST = 60, CHECKPOINT = 61, COLUMN = 62, 
    COMMENT = 63, COMMIT = 64, COMMIT_SKIP_CHECKPOINT = 65, CONTAINS = 66, 
    COPY = 67, COUNT = 68, CREATE = 69, CSR = 70, CYCLE = 71, DATABASE = 72, 
    DBTYPE = 73, DEFAULT = 74, DELETE = 75, DESC = 76, DESCENDING = 77, 
    DETACH = 78, DISTINCT = 79, DROP = 80, ELSE = 81, END = 82, EVERY = 83, 
    ENDS = 84, EXISTS = 85, EXPLAIN = 86, EXPORT = 87, EXTENSION = 88, FALSE = 89, 
    FROM = 90, FORCE = 91, FOR = 92, GLOB = 93, GRAPH = 94, GROUP = 95, 
    HEADERS = 96, HINT = 97, IMPORT = 98, INDEX = 99, IF = 100, IN = 101, 
    INCREMENT = 102, INSTALL = 103, IS = 104, JOIN = 105, KEY = 106, LIMIT = 107, 
    LOAD = 108, LOGICAL = 109, MACRO = 110, MATCH = 111, MAXVALUE = 112, 
    MERGE = 113, MINVALUE = 114, MULTI_JOIN = 115, NO = 116, NODE = 117, 
    NOT = 118, NONE = 119, NULL_ = 120, ON = 121, ONLY = 122, OPTIONS = 123, 
    OPTIONAL = 124, OR = 125, ORDER = 126, PRIMARY = 127, PROFILE = 128, 
    PROJECT = 129, READ = 130, REL = 131, RENAME = 132, RETURN = 133, ROLLBACK = 134, 
    ROLLBACK_SKIP_CHECKPOINT = 135, SEQUENCE = 136, SET = 137, SORTED = 138, 
    SHORTEST = 139, START = 140, STARTS = 141, STRUCT = 142, TABLE = 143, 
    THEN = 144, TO = 145, TRAIL = 146, TRANSACTION = 147, TUMBLE = 148, 
    TRUE = 149, TYPE = 150, UNION = 151, UNWIND = 152, UNINSTALL = 153, 
    UPDATE = 154, USE = 155, WHEN = 156, WHERE = 157, WITH = 158, WRITE = 159, 
    WSHORTEST = 160, XOR = 161, SINGLE = 162, YIELD = 163, USER = 164, PASSWORD = 165, 
    ROLE = 166, MAP = 167, DECIMAL = 168, STAR = 169, L_SKIP = 170, INVALID_NOT_EQUAL = 171, 
    COLON = 172, DOTDOT = 173, MINUS = 174, FACTORIAL = 175, StringLiteral = 176, 
    EscapedChar = 177, DecimalInteger = 178, HexLetter = 179, HexDigit = 180, 
    Digit = 181, NonZeroDigit = 182, NonZeroOctDigit = 183, ZeroDigit = 184, 
    ExponentDecimalReal = 185, RegularDecimalReal = 186, UnescapedSymbolicName = 187, 
    IdentifierStart = 188, IdentifierPart = 189, EscapedSymbolicName = 190, 
    SP = 191, WHITESPACE = 192, CypherComment = 193, Unknown = 194
  };

  explicit CypherLexer(antlr4::CharStream *input);

  ~CypherLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

