#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tags.h"

/*
 * Change the following initialization for the default output language
 */
#ifdef ENGLISH
int default_language = ENGLISH_LANGUAGE;
#endif

struct tag gedcom_tags[] = {
  {"ADDR", ADDR, {"ADDRESS"}},
  {"ADOP", ADOP, {"ADOPTION"}},
  {"AFN", AFN, {"AFN"}},
  {"AGE", AGE, {"AGE"}},
  {"AGNC", AGNC, {"AGENCY"}},
  {"ALIA", ALIA, {"ALIAS"}},
  {"ANCI", ANCI, {"ANCI"}},
  {"ANUL", ANUL, {"ANNULMENT"}},
  {"ARVL", ARVL, {"ARRIVAL"}},
  {"ASSO", ASSO, {"ASSOCIATES"}},
  {"AUTH", AUTH, {"AUTHOR"}},
  {"BAPL", BAPL, {"BAPTISM-LDS"}},
  {"BAPM", BAPM, {"BAPTISM"}},
  {"BARM", BARM, {"BAR MITZVAH"}},
  {"BASM", BASM, {"BAS MITZVAH"}},
  {"BIRT", BIRT, {"BIRTH"}},
  {"BLES", BLES, {"BLESSING"}},
  {"BROT", BROT, {"BROTHER"}},
  {"BURI", BURI, {"BURIAL"}},
  {"BUYR", BUYR, {"BUYER"}},
  {"CALN", CALN, {"CALL NUMBER"}},
  {"CAST", CAST, {"CASTE"}},
  {"CAUS", CAUS, {"CAUSE"}},
  {"CEME", CEME, {"CEMETERY"}},
  {"CENS", CENS, {"CENSUS"}},
  {"CHAN", CHAN, {"CHANGE"}},
  {"CHAR", CHAR, {"CHARACTER"}},
  {"CHIL", CHIL, {"CHILD"}},
  {"CHR", CHR, {"CHRISTENING"}},
  {"CHRA", CHRA, {"ADULT CHRISTENING"}},
  {"CLAS", CLAS, {"CLASSIFICATION"}},
  {"COMM", COMM, {"CONTACT PERSON"}},
  {"CONC", CONC, {"CONCATENATION"}},
  {"CONF", CONF, {"CONFIRMATION"}},
  {"CONL", CONL, {"CONFIRMATION LDS"}},
  {"CONT", CONT, {"CONTINUED"}},
  {"COPR", COPR, {"COPYRIGHT"}},
  {"CORP", CORP, {"CORPORATION"}},
  {"CPLR", CPLR, {"COMPILER"}},
  {"DATA", DATA, {"DATA"}},
  {"DATE", DATE, {"DATE"}},
  {"DEAT", DEAT, {"DEATH"}},
  {"DEFN", DEFN, {"DEFINITION"}},
  {"DESI", DESI, {"DESI"}},
  {"DEST", DEST, {"DESTINATION"}},
  {"DIV", DIV, {"DIVORCE"}},
  {"DIVF", DIVF, {"DIVORCE FILED"}},
  {"DPRT", DPRT, {"DEPARTURE"}},
  {"DSCR", DSCR, {"DESCRIPTION"}},
  {"EDTR", EDTR, {"EDITOR"}},
  {"EDUC", EDUC, {"EDUCATION"}},
  {"EMIG", EMIG, {"EMIGRATION"}},
  {"ENDL", ENDL, {"ENDOWMENT LDS"}},
  {"ENGA", ENGA, {"ENGAGEMENT"}},
  {"EVEN", EVEN, {"EVENT"}},
  {"FAM", FAM, {"FAMILY"}},
  {"FAMC", FAMC, {"FAMILY CHILD"}},
  {"FAMS", FAMS, {"FAMILY SPOUSE"}},
  {"FATH", FATH, {"FATHER"}},
  {"FIDE", FIDE, {"FIDELITY"}},
  {"FILM", FILM, {"FILM"}},
  {"FILE", FYLE, {"FILE"}},
  {"FORM", FORM, {"FORMAT"}},
  {"GEDC", GEDC, {"GEDCOM"}},
  {"GODP", GODP, {"GODPARENT"}},
  {"GRAD", GRAD, {"GRAD"}},
  {"HDOH", HDOH, {"HEAD OF HOUSEHOLD"}},
  {"HEAD", HEAD, {"HEADER"}},
  {"HEIR", HEIR, {"HEIR"}},
  {"HFAT", HFAT, {"HUSBAND FATHER"}},
  {"HMOT", HMOT, {"HUSBAND MOTHER"}},
  {"HUSB", HUSB, {"HUSBAND"}},
  {"IDNO", IDNO, {"IDENT NUMBER"}},
  {"IMMI", IMMI, {"IMMIGRATION"}},
  {"INDI", INDI, {"INDIVIDUAL"}},
  {"INDX", INDX, {"INDEXED"}},
  {"INFT", INFT, {"INFORMANT"}},
  {"INTV", INTV, {"INTERVIEWER"}},
  {"ISA", ISA, {"IS A"}},
  {"ISSUE", ISSUE, {"ISSUE"}},
  {"ITEM", ITEM, {"ITEM"}},
  {"LABL", LABL, {"LABEL"}},
  {"LANG", LANG, {"LANGUAGE"}},
  {"LCCN", LCCN, {"LCCN"}},
  {"LGTE", LGTE, {"LEGATEE"}},
  {"MARB", MARB, {"MARRIAGE BANN"}},
  {"MARC", MARC, {"MARRIAGE CONTRACT"}},
  {"MARL", MARL, {"MARRIAGE LICENSE"}},
  {"MARR", MARR, {"MARRIAGE"}},
  {"MARS", MARS, {"MARRIAGE SETTLEMENT"}},
  {"MBR", MBR, {"MEMBER"}},
  {"MEDI", MEDI, {"MEDIA"}},
  {"MOTH", MOTH, {"MOTHER"}},
  {"NAME", NAME, {"NAME"}},
  {"NAMR", NAMR, {"NAMR"}},
  {"NAMS", NAMS, {"NAMESAKE"}},
  {"NATI", NATI, {"NATIONALITY"}},
  {"NATU", NATU, {"NATURALIZATION"}},
  {"NCHI", NCHI, {"NO. OF CHILDREN"}},
  {"NMR", NMR, {"NO. OF MARRIAGES"}},
  {"NOTE", NOTE, {"NOTE"}},
  {"NUMB", NUMB, {"NUMBER"}},		/* This one's not official */
  {"OCCU", OCCU, {"OCCUPATION"}},
  {"OFFI", OFFI, {"OFFICIATOR"}},
  {"ORDN", ORDN, {"ORDINATION"}},
  {"ORIG", ORIG, {"ORIGINATION"}},
  {"OWNR", OWNR, {"OWNER"}},
  {"PAGE", PAGE, {"PAGE"}},
  {"PARI", PARI, {"PERIOD"}},
  {"PHON", PHON, {"PHONE"}},
  {"PHOTO", PHOTO, {"PHOTO"}},
  {"PHUS", PHUS, {"PREVIOUS HUSBAND"}},
  {"PLAC", PLAC, {"PLACE"}},
  {"PORT", PORT, {"PORT"}},
  {"PROB", PROB, {"PROBATE"}},
  {"PROP", PROP, {"PROPERTY"}},
  {"PUBL", PUBL, {"PUBLICATION"}},
  {"PUBR", PUBR, {"PUBLISHER"}},
  {"PWIF", PWIF, {"PREVIOUS WIFE"}},
  {"QUAY", QUAY, {"QUALITY"}},
  {"RECO", RECO, {"RECORDER"}},
  {"REFN", REFN, {"REFERENCE"}},
  {"REFS", REFS, {"REFERENCED SOURCE"}},
  {"RELI", RELI, {"RELIGION"}},
  {"REPO", REPO, {"REPOSITORY"}},
  {"RETI", RETI, {"RETIREMENT"}},
  {"RFN", RFN, {"RFN"}},
  {"ROLE", ROLE, {"ROLE"}},
  {"SCHEMA", SCHEMA, {"SCHEMA"}},
  {"SELR", SELR, {"SELLER"}},
  {"SEQU", SEQU, {"SEQUENCE"}},
  {"SERS", SERS, {"SERIES"}},
  {"SEX", SEX, {"SEX"}},
  {"SIBL", SIBL, {"SIBLING"}},
  {"SIGN", SIGN, {"SIGNATURE"}},
  {"SIST", SIST, {"SISTER"}},
  {"SITE", SITE, {"SITE"}},
  {"SLGC", SLGC, {"SLGC"}},
  {"SLGS", SLGS, {"SLGS"}},
  {"SOUND", SOUND, {"SOUND"}},
  {"SOUR", SOUR, {"SOURCE"}},
  {"SPOU", SPOU, {"SPOUSE"}},
  {"SSN", SSN, {"SSN"}},
  {"STAT", STAT, {"STATUS"}},
  {"SUBM", SUBM, {"SUBMITTER"}},
  {"SUBN", SUBN, {"SUBN"}},
  {"TEMP", TEMP, {"TEMPLE"}},
  {"TEXT", TEXT, {"TEXT"}},
  {"TIME", TIME, {"TIME"}},
  {"TITL", TITL, {"TITLE"}},
  {"TRLR", TRLR, {"TRAILER"}},
  {"TXPY", TXPY, {"TAXPAYER"}},
  {"TYPE", TYPE, {"TYPE"}},
  {"VERS", VERS, {"VERSION"}},
  {"WFAT", WFAT, {"WIFE FATHER"}},
  {"WIFE", WIFE, {"WIFE"}},
  {"WILL", WILL, {"WILL"}},
  {"WITN", WITN, {"WITNESS"}},
  {"WMOT", WMOT, {"WIFE MOTHER"}},
  {"XLTR", XLTR, {"TRANSLATOR"}},
};

int gedcom_tags_size = sizeof(gedcom_tags)/sizeof(struct tag);

struct tag gedcom_special[] = {
  {"ABSTRACTOR", ABSTRACTOR, {"ABSTRACTOR"}},
  {"ABT", ABT, {"ABOUT"}},
  {"ACTIVE", ACTIVE, {"ACTIVE"}},
  {"AFT", AFT, {"AFTER"}},
  {"AND", AND, {"AND"}},
  {"AUDIO", AUDIO, {"AUDIO"}},
  {"AUTHOR", AUTHOR, {"AUTHOR"}},
  {"BC", BC, {"BC"}},
  {"BEF", BEF, {"BEFORE"}},
  {"BET", BET, {"BETWEEN"}},
  {"BIC", BIC, {"BIC"}},
  {"BOOK", BOOK, {"BOOK"}},
  {"BUSINESS", BUSINESS, {"BUSINESS"}},
  {"CANCELED", CANCELED, {"CANCELED"}},
  {"CARD", CARD, {"CARD"}},
  {"CENSUS", CENSUS, {"CENSUS"}},
  {"CHURCH", CHURCH, {"CHURCH"}},
  {"COMPILER", COMPILER, {"COMPILER"}},
  {"COMPLETED", COMPLETED, {"COMPLETED"}},
  {"COPY", COPY, {"COPY"}},
  {"COURT", COURT, {"COURT"}},
  {"CTNC", CTNC, {"CTNC"}},
  {"DIVORCED", DIVORCED, {"DIVORCED"}},
  {"DNS", DNS, {"DNS"}},
  {"DONE", DONE, {"DONE"}},
  {"DWEL", DWEL, {"DWELLING"}},
  {"EDITOR", EDITOR, {"EDITOR"}},
  {"EDTN", EDTN, {"EDITION"}},
  {"ELECTRONIC", ELECTRONIC, {"ELECTRONIC"}},
  {"EST", EST, {"EST"}},
  {"EXTRACT", EXTRACT, {"EXTRACT"}},
  {"F", FEMALE, {"FEMALE"}},
  {"FAMN", FAMN, {"FAMILY NUMBER"}},
  {"FICHE", FICHE, {"FICHE"}},
  {"FOUND", FOUND, {"FOUND"}},
  {"FROM", FROM, {"FROM"}},
  {"GOVERNMENT", GOVERNMENT, {"GOVERNMENT"}},
  {"HDOG", HDOG, {"HDOG"}},
  {"HISTORY", HISTORY, {"HISTORY"}},
  {"INFANT", INFANT, {"INFANT"}},
  {"INFORMANT", INFORMANT, {"INFORMANT"}},
  {"INTERVIEW", INTERVIEW, {"INTERVIEW"}},
  {"INTERVIEWER", INTERVIEWER, {"INTERVIEWER"}},
  {"ISSU", ISSU, {"ISSUE"}},
  {"JOURNAL", JOURNAL, {"JOURNAL"}},
  {"LAND", LAND, {"LAND"}},
  {"LEGA", LEGA, {"LEGAL"}},
  {"LETTER", LETTER, {"LETTER"}},
  {"M", MALE, {"MALE"}},
  {"MAGAZINE", MAGAZINE, {"MAGAZINE"}},
  {"MANUSCRIPT", MANUSCRIPT, {"MANUSCRIPT"}},
  {"MAP", MAP, {"MAP"}},
  {"MARRIED", MARRIED, {"MARRIED"}},
  {"MEMBER", MEMBER, {"MEMBER"}},
  {"MILITARY", MILITARY, {"MILITARY"}},
  {"MSTAT", MSTAT, {"MSTAT"}},
  {"NEWSPAPER", NEWSPAPER, {"NEWSPAPER"}},
  {"NO", NO, {"NO"}},
  {"NUMBER", NUMBER, {"NUMBER"}},
  {"ORDERED", ORDERED, {"ORDERED"}},
  {"ORGANIZATION", ORGANIZATION, {"ORGANIZATION"}},
  {"ORIGINAL", ORIGINAL, {"ORIGINAL"}},
  {"PARE", PARE, {"PARENT"}},
  {"PERI", PERI, {"PERIOD"}},
  {"PERIODICAL", PERIODICAL, {"PERIODICAL"}},
  {"PERSONAL", PERSONAL, {"PERSONAL"}},
  {"PHOTOCOPY", PHOTOCOPY, {"PHOTOCOPY"}},
  {"PLACE", PLACE, {"PLACE"}},
  {"PLANNED", PLANNED, {"PLANNED"}},
  {"PLOT", PLOT, {"PLOT"}},
  {"PROVED", PROVED, {"PROVED"}},
  {"RECITED", RECITED, {"RECITED"}},
  {"REL", REL, {"REL"}},
  {"SHEE", SHEE, {"SHEET"}},
  {"SINGLE", SINGLE, {"SINGLE"}},
  {"STILLBORN", STILLBORN, {"STILLBORN"}},
  {"SUBMITTED", SUBMITTED, {"SUBMITTED"}},
  {"TO", TO, {"TO"}},
  {"TOMBSTONE", TOMBSTONE, {"TOMBSTONE"}},
  {"TRADITION", TRADITION, {"TRADITION"}},
  {"TRANSCRIBER", TRANSCRIBER, {"TRANSCRIBER"}},
  {"TRANSCRIPT", TRANSCRIPT, {"TRANSCRIPT"}},
  {"UNPUBLISHED", UNPUBLISHED, {"UNPUBLISHED"}},
  {"USB", USB, {"USB"}},
  {"VIDEO", VIDEO, {"VIDEO"}},
  {"VITAL", VITAL, {"VITAL"}},
  {"WAC", WAC, {"WAC"}},
  {"WIDOWED", WIDOWED, {"WIDOWED"}},
  {"YES", YES, {"YES"}},
};

int gedcom_special_size = sizeof(gedcom_special)/sizeof(struct tag);

struct tag gedcom_other[] = {
  {"ANSEL", ANSEL, {"ANSEL"}},
  {"BEGIN", BEGIN, {"BEGIN"}},
  {"COLON", COLON, {"COLON"}},
  {"END", END, {"END"}},
  {"ESCAPE_SEQUENCE", ESCAPE_SEQUENCE, {"ESCAPE SEQUENCE"}},
  {"IBMPC", ANSEL, {"IBMPC"}}, /* Output by Family Scrapbook */
  {"LINE", LINE, {"LINE"}},
  {"LINEAGE-LINKED", LINEAGE_LINKED, {"LINEAGE LINKED"}},
  {"LINEAGE_LINKED", LINEAGE_LINKED, {"LINEAGE LINKED"}},
  {"OTHER_BANG", OTHER_BANG, {"OTHER BANG"}},
  {"SLASH", SLASH, {"SLASH"}},
  {"TOKEN", TOKEN, {"TOKEN"}},
  {"UNDERSCORE", UNDERSCORE, {"UNDERSCORE"}},
  {"UNICODE", UNICODE, {"UNICODE"}},
  {"XREF", XREF, {"XREF"}},
};

int gedcom_other_size = sizeof(gedcom_other)/sizeof(struct tag);

int mystrcmp(char *a, struct tag *b)
{
  return(strcmp(a, b->name));
}

struct tag *findtag(char *s, struct tag *tab, int nmemb)
{
  struct tag *t;
  t = bsearch(s, tab, nmemb, sizeof(struct tag),
		 (int (*)(const void *, const void *))mystrcmp);
  return(t);
}

struct tag *validate_tags_table(struct tag *tab, int nmemb)
{
  struct tag *prev = NULL;
  while(nmemb--) {
    if(prev && strcmp(prev->name, tab->name) >= 0)
      return(tab);
    prev = tab++;
  }
  return(NULL);
}

void validate_tags_tables()
{
  struct tag *tab;
  if(tab = validate_tags_table(gedcom_tags, gedcom_tags_size)) {
    fprintf(stderr, "Internal error: GEDCOM tag table out of order at tag '%s'.\n",
	    tab->name);
    exit(1);
  }
  if(tab = validate_tags_table(gedcom_special, gedcom_special_size)) {
    fprintf(stderr, "Internal error: special tag table out of order at tag '%s'.\n",
	    tab->name);
    exit(1);
  }
  if(tab = validate_tags_table(gedcom_other, gedcom_other_size)) {
    fprintf(stderr, "Internal error: 'other' tag table out of order at tag '%s'.\n",
	    tab->name);
    exit(1);
  }
}
