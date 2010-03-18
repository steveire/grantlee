/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LOADERTAGSTEST_H
#define LOADERTAGSTEST_H

#include <QtTest>
#include <QObject>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "grantlee_paths.h"


typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestLoaderTags : public QObject
{
  Q_OBJECT

private slots:
  void initTestCase();
  void cleanupTestCase();

  void testIncludeTag_data();
  void testIncludeTag() {
    doTest();
  }

  void testExtendsTag_data();
  void testExtendsTag() {
    doTest();
  }

private:

  void doTest();

  InMemoryTemplateLoader::Ptr loader;
  Engine *m_engine;

};

void TestLoaderTags::initTestCase()
{
  m_engine = new Engine( this );

  loader = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );
  m_engine->addTemplateLoader( loader );

  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginDirs( QStringList() << GRANTLEE_PLUGIN_PATH
                                         << ":/grantlee/" // For testtags.qs
                         );
}

void TestLoaderTags::cleanupTestCase()
{
  delete m_engine;
}

void TestLoaderTags::doTest()
{
  QFETCH( QString, input );
  QFETCH( Dict, dict );
  QFETCH( QString, output );
  QFETCH( Grantlee::Error, errorNumber );

  Template t = m_engine->newTemplate( input, QTest::currentDataTag() );

  Context context( dict );

  QString result = t->render( &context );

  if ( t->error() != NoError ) {
    QCOMPARE( t->error(), errorNumber );
    return;
  }

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE( NoError, errorNumber );

  QCOMPARE( t->error(), NoError );

  QCOMPARE( result, output );
}

void TestLoaderTags::testIncludeTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;

  loader->setTemplate( "basic-syntax01", "Something cool" );
  loader->setTemplate( "basic-syntax02", "{{ headline }}" );

  QTest::newRow( "include01" ) << "{% include \"basic-syntax01\" %}" << dict << "Something cool" << NoError;

  dict.insert( "headline", "Included" );

  QTest::newRow( "include02" ) << "{% include \"basic-syntax02\" %}" << dict << "Included" << NoError;

  dict.insert( "templateName", "basic-syntax02" );

  QTest::newRow( "include03" ) << "{% include templateName %}" << dict << "Included" << NoError;

  dict.clear();
  QTest::newRow( "include04" ) << "a{% include \"nonexistent\" %}b" << dict << "ab" << NoError;

  QString incl05 = "template with a space";
  loader->setTemplate( "include 05", incl05 );

  QTest::newRow( "include 05" ) << incl05 << dict << "template with a space" << NoError;

  QTest::newRow( "include06" ) << "{% include \"include 05\" %}" << dict << "template with a space" << NoError;

  dict.clear();
  dict.insert( "list", QVariantList() << QVariant() << QVariant() );
  QTest::newRow( "include07" ) << "{% for i in list %}{% include \"include 05\" %}{% endfor %}" << dict << "template with a spacetemplate with a space" << NoError;

}


void TestLoaderTags::testExtendsTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "errorNumber" );

  Dict dict;
  // Basic test
  QTest::newRow( "namedendblocks01" ) << "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock first %}3" << dict << "1_2_3" << NoError;
  // Unbalanced blocks
  QTest::newRow( "namedendblocks02" ) << "1{% block first %}_{% block second %}2{% endblock first %}_{% endblock second %}3" << dict << "" << InvalidBlockTagError;
  QTest::newRow( "namedendblocks03" ) << "1{% block first %}_{% block second %}2{% endblock %}_{% endblock second %}3" << dict << "" << InvalidBlockTagError;
  QTest::newRow( "namedendblocks04" ) << "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock third %}3" << dict << "" << InvalidBlockTagError;
  QTest::newRow( "namedendblocks05" ) << "1{% block first %}_{% block second %}2{% endblock first %}" << dict << "" << InvalidBlockTagError;
  // Mixed named and unnamed endblocks
  QTest::newRow( "namedendblocks06" ) << "1{% block first %}_{% block second %}2{% endblock %}_{% endblock first %}3" << dict << "1_2_3" << NoError;
  QTest::newRow( "namedendblocks07" ) << "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock %}3" << dict << "1_2_3" << NoError;


  //## INHERITANCE ###########################################################
  // Standard template with no inheritance

  QString inh1( "1{% block first %}&{% endblock %}3{% block second %}_{% endblock %}" );
  loader->setTemplate( "inheritance01", inh1 );

  QTest::newRow( "inheritance01" ) << inh1 << dict << "1&3_" << NoError;

  QString inh2( "{% extends \"inheritance01\" %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" );
  loader->setTemplate( "inheritance02", inh2 );

  // Standard two-level inheritance
  QTest::newRow( "inheritance02" ) << inh2 << dict << "1234" << NoError;
  // Three-level with no redefinitions on third level
  QTest::newRow( "inheritance03" ) << "{% extends 'inheritance02' %}" << dict << "1234" << NoError;
  // Two-level with no redefinitions on second level

  QString inh4( "{% extends \"inheritance01\" %}" );
  loader->setTemplate( "inheritance04", inh4 );

  QTest::newRow( "inheritance04" ) << inh4 << dict << "1&3_" << NoError;
  // Two-level with double quotes instead of single quotes
  QTest::newRow( "inheritance05" ) << "{% extends \"inheritance02\" %}" << dict << "1234" << NoError;

  dict.insert( "foo", "inheritance02" );
  // Three-level with variable parent-template name
  QTest::newRow( "inheritance06" ) << "{% extends foo %}" << dict << "1234" << NoError;

  QString inh7( "{% extends 'inheritance01' %}{% block second %}5{% endblock %}" );
  loader->setTemplate( "inheritance07", inh7 );

  dict.clear();
  // Two-level with one block defined, one block not defined
  QTest::newRow( "inheritance07" ) << inh7 << dict << "1&35" << NoError;
  // Three-level with one block defined on this level, two blocks defined next level
  QTest::newRow( "inheritance08" ) << "{% extends 'inheritance02' %}{% block second %}5{% endblock %}" << dict << "1235" << NoError;

  // Three-level with second and third levels blank
  QTest::newRow( "inheritance09" ) << "{% extends 'inheritance04' %}" << dict << "1&3_" << NoError;

  dict.clear();

  // Three-level with space NOT in a block -- should be ignored
  QTest::newRow( "inheritance10" ) << "{% extends 'inheritance04' %}      " << dict << "1&3_" << NoError;
  // Three-level with both blocks defined on this level, but none on second level
  QTest::newRow( "inheritance11" ) << "{% extends 'inheritance04' %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" << dict << "1234" << NoError;
  // Three-level with this level providing one and second level providing the other
  QTest::newRow( "inheritance12" ) << "{% extends 'inheritance07' %}{% block first %}2{% endblock %}" << dict << "1235" << NoError;
  // Three-level with this level overriding second level
  QTest::newRow( "inheritance13" ) << "{% extends 'inheritance02' %}{% block first %}a{% endblock %}{% block second %}b{% endblock %}" << dict << "1a3b" << NoError;
  // A block defined only in a child template shouldn't be displayed
  QTest::newRow( "inheritance14" ) << "{% extends 'inheritance01' %}{% block newblock %}NO DISPLAY{% endblock %}" << dict << "1&3_" << NoError;


  QString inh15( "{% extends 'inheritance01' %}{% block first %}2{% block inner %}inner{% endblock %}{% endblock %}" );
  loader->setTemplate( "inheritance15", inh15 );

  // A block within another block
  QTest::newRow( "inheritance15" ) << inh15 << dict << "12inner3_" << NoError;
  // A block within another block (level 2)

  QTest::newRow( "inheritance16" ) << "{% extends 'inheritance15' %}{% block inner %}out{% endblock %}" << dict << "12out3_" << NoError;

  // {% load %} tag (parent -- setup for exception04)
  QString inh17( "{% load testtags %}{% block first %}1234{% endblock %}" );
  loader->setTemplate( "inheritance17", inh17 );

  dict.clear();
  QTest::newRow( "inheritance17" ) << inh17 << dict << "1234" << NoError;

  // {% load %} tag (standard usage, without inheritance)
  QTest::newRow( "inheritance18" ) << "{% load testtags %}{% echo this that theother %}5678" << dict << "this that theother5678" << NoError;

  // {% load %} tag (within a child template)
  QTest::newRow( "inheritance19" ) << "{% extends 'inheritance01' %}{% block first %}{% load testtags %}{% echo 400 %}5678{% endblock %}" << dict << "140056783_" << NoError;

  QString inh20( "{% extends 'inheritance01' %}{% block first %}{{ block.super }}a{% endblock %}" );
  loader->setTemplate( "inheritance20", inh20 );

  // Two-level inheritance with {{ block.super }}
  QTest::newRow( "inheritance20" ) << inh20 << dict << "1&a3_" << NoError;
  // Three-level inheritance with {{ block.super }} from parent
  QTest::newRow( "inheritance21" ) << "{% extends 'inheritance02' %}{% block first %}{{ block.super }}a{% endblock %}" << dict << "12a34" << NoError;
  // Three-level inheritance with {{ block.super }} from grandparent
  QTest::newRow( "inheritance22" ) << "{% extends 'inheritance04' %}{% block first %}{{ block.super }}a{% endblock %}" << dict << "1&a3_" << NoError;
  // Three-level inheritance with {{ block.super }} from parent and grandparent
  QTest::newRow( "inheritance23" ) << "{% extends 'inheritance20' %}{% block first %}{{ block.super }}b{% endblock %}" << dict << "1&ab3_" << NoError;

  // Inheritance from local context without use of template loader

  Template t = m_engine->newTemplate( "1{% block first %}_{% endblock %}3{% block second %}_{% endblock %}", "context_template" );
  dict.insert( "context_template", QVariant::fromValue( t ) );

  QTest::newRow( "inheritance24" ) << "{% extends context_template %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" << dict << "1234" << NoError;

  dict.clear();
  QVariantList list;

  Template t1 = m_engine->newTemplate( "Wrong", "context_template_1" );
  Template t2 = m_engine->newTemplate( "1{% block first %}_{% endblock %}3{% block second %}_{% endblock %}", "context_template_2" );
  list << QVariant::fromValue( t1 );
  list << QVariant::fromValue( t2 );

  dict.insert( "context_template", list );

  // Inheritance from local context with variable parent template
  QTest::newRow( "inheritance25" ) << "{% extends context_template.1 %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" << dict << "1234" << NoError;

  dict.clear();

  // Set up a base template to extend
  QString inh26 = QString( "no tags" );
  loader->setTemplate( "inheritance26", inh26 );

  // Inheritance from a template that doesn't have any blocks
  QTest::newRow( "inheritance27" ) << "{% extends 'inheritance26' %}" << dict << "no tags" << NoError;

  QString inh28 = "{% block first %}!{% endblock %}";
  loader->setTemplate( "inheritance 28", inh28 );

  QTest::newRow( "inheritance 28" ) << inh28 << dict << "!" << NoError;

  // Inheritance from a template with a space in its name should work.
  QTest::newRow( "inheritance29" ) << "{% extends 'inheritance 28' %}" << dict << "!" << NoError;

  dict.insert( "list", QVariantList() << "One" << "Two" << "Three" );

  // Tags that affect rendering have no afftect outside of a block in an extension template
  // (and therefore "i" is undefined here)
  // {% load %} tags for example are ok in child templates because they are processed at
  // compile-time, not at rendering time.
  QTest::newRow( "inheritance30" ) << "{% extends 'inheritance01' %}"
      "{% for i in list %}{% block first %}A{{ i }}B{% endblock %}{% endfor %}" << dict << "1AB3_" << NoError;

  QString inh31 = "{% extends 'inheritance01' %}{% block first %}2{% block second %}4{% endblock %}{% endblock %}";
  loader->setTemplate( "inheritance31", inh31 );

  // If blocks are siblings in a parent template and defined as children in an extension template,
  // the 'moved' block is rendered in its position in its parent template and in its position
  // in the extended template
  QTest::newRow( "inheritance31" ) << inh31 << dict << "12434" << NoError;

  QString inh32 = "{% extends 'inheritance01' %}{% block second %}4{% block first %}2{% endblock %}{% endblock %}";
  loader->setTemplate( "inheritance32", inh32 );

  // The order of appearance of the blocks does not determine this.
  QTest::newRow( "inheritance32" ) << inh32 << dict << "12342" << NoError;

  QTest::newRow( "inheritance33" ) << "{% extends 'inheritance01' %}"
      "{% block first %}2{% block second %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << "12AB3A_B" << NoError;

  QTest::newRow( "inheritance34" ) << "{% extends 'inheritance01' %}"
      "{% block second %}4{% block first %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << "1A&B34AB" << NoError;

  // Looping a block inside another block causes the block to be rendered multiple times. Once again,
  // When {{ i }} is rendered outside of the "first" block, "i" is not in context
  QTest::newRow( "inheritance35" ) << "{% extends 'inheritance01' %}"
      "{% block first %}"
      "A{% for i in list %}{% block second %}~{{ i }}:{% endblock %}{% endfor %}B"
      "{% endblock %}" << dict << "1A~One:~Two:~Three:B3~:" << NoError;

  QString inh36 = "1{% block first %}&{% endblock %}3{% block second %}_{% endblock %}5{% block third %}+{% endblock %}";
  loader->setTemplate( "inheritance36", inh36 );

  QTest::newRow( "inheritance36" ) << inh36 << dict << "1&3_5+" << NoError;

  QString inh37 = "{% extends 'inheritance36' %}{% block second %}4{% block third %}6{% endblock %}{% endblock %}";
  loader->setTemplate( "inheritance37", inh37 );

  // The second contains the third:
  QTest::newRow( "inheritance37" ) << inh37 << dict << "1&34656" << NoError;

  // The first contains the second, contains the third:
  QTest::newRow( "inheritance38" ) << "{% extends 'inheritance37' %}"
      "{% block first %}2{% block second %}4{% block third %}6{% endblock %}{% endblock %}{% endblock %}" << dict << "124634656" << NoError;

  // ### Unexpected behaviour: we get "12AB3AABB56" here.
//  QTest::newRow( "inheritance39" ) << "{% extends 'inheritance37' %}"
//      "{% block first %}2{% block second %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << "12A46B3A46B56" << NoError;

  // When {% block second %}{{ block.super }}{% endblock %} is rendered in the parent position (inheritance35)
  QTest::newRow( "inheritance40" ) << "{% extends 'inheritance37' %}"
      "{% block first %}2{% block third %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << "12AB34AB5AABB" << NoError;

  // ### Unexpected behaviour: we get "1&3A46ABB56AB" here
  // Actually, this is an infinite loop...
//  QTest::newRow( "inheritance41" ) << "{% extends 'inheritance37' %}"
//      "{% block third %}6{% block second %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << "1&3A46B56A46B" << NoError;

  // ### Unexpected behaviour: we get "1A&B346AB56AB" here
//  QTest::newRow( "inheritance42" ) << "{% extends 'inheritance37' %}"
//      "{% block third %}6{% block first %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << "1A&B346A&B56A&B" << NoError;

  QString inh43 = "A{% block first %}B{% block second %}C{% endblock %}D{% endblock %}E";
  loader->setTemplate( "inheritance43", inh43 );

  QTest::newRow( "inheritance43" ) << inh43 << dict << "ABCDE" << NoError;

  QTest::newRow( "inheritance44" ) << "{% extends 'inheritance43' %}"
      "{% block first %}1{% endblock %}" << dict << "A1E" << NoError;

  QTest::newRow( "inheritance45" ) << "{% extends 'inheritance43' %}"
      "{% block first %}1{{ block.super }}2{% endblock %}" << dict << "A1BCD2E" << NoError;

  QTest::newRow( "inheritance46" ) << "{% extends 'inheritance43' %}"
      "{% block second %}2{% endblock %}" << dict << "AB2DE" << NoError;

  QTest::newRow( "inheritance47" ) << "{% extends 'inheritance43' %}"
      "{% block second %}2{{ block.super }}3{% endblock %}" << dict << "AB2C3DE" << NoError;

  // Although block second is overridden, it is not rendered because first is overridden excluding it.
  QTest::newRow( "inheritance48" ) << "{% extends 'inheritance43' %}"
      "{% block first %}1{% endblock %}{% block second %}2{% endblock %}" << dict << "A1E" << NoError;

  // Block.super, which uses block second, renders the overridden block second.
  QTest::newRow( "inheritance49" ) << "{% extends 'inheritance43' %}"
      "{% block first %}{{ block.super }}{% endblock %}{% block second %}2{% endblock %}" << dict << "AB2DE" << NoError;

  QTest::newRow( "inheritance50" ) << "{% extends 'inheritance43' %}"
      "{% block first %}{{ block.super }}{% endblock %}{% block second %}2{{ block.super }}3{% endblock %}" << dict << "AB2C3DE" << NoError;

  // ### Unexpected result: We get A1234E here. block.super doesn't work if overriding in a nested block I guess.
//  QTest::newRow( "inheritance51" ) << "{% extends 'inheritance43' %}"
//      "{% block first %}1{% block second %}2{{ block.super }}3{% endblock %}4{% endblock %}" << dict << "A12C34E" << NoError;

  dict.clear();
  // Raise exception for invalid template name
  QTest::newRow( "exception01" ) << "{% extends 'nonexistent' %}" << dict << "" << TagSyntaxError;
  // Raise exception for invalid template name (in variable)
  QTest::newRow( "exception02" ) << "{% extends nonexistent %}" << dict << "" << TagSyntaxError;
  // Raise exception for extra {% extends %} tags
  QTest::newRow( "exception03" ) << "{% extends 'inheritance01' %}{% block first %}2{% endblock %}{% extends 'inheritance16' %}" << dict << "" << TagSyntaxError;
  // Raise exception for custom tags used in child with {% load %} tag in parent, not in child
  QTest::newRow( "exception04" ) << "{% extends 'inheritance17' %}{% block first %}{% echo 400 %}5678{% endblock %}" << dict << "" << InvalidBlockTagError;
}


QTEST_MAIN( TestLoaderTags )
#include "testloadertags.moc"

#endif

