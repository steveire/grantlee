/*
  This file is part of the Grantlee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef LOADERTAGSTEST_H
#define LOADERTAGSTEST_H

#include <QtTest/QTest>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#include "template.h"
#include "engine.h"
#include "context.h"
#include "grantlee_paths.h"
#include "coverageobject.h"


typedef QHash<QString, QVariant> Dict;

Q_DECLARE_METATYPE( Dict )
Q_DECLARE_METATYPE( Grantlee::Error )

using namespace Grantlee;

class TestLoaderTags : public CoverageObject
{
  Q_OBJECT

private Q_SLOTS:
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
  Q_INIT_RESOURCE(testresource);

  m_engine = new Engine( this );

  loader = InMemoryTemplateLoader::Ptr( new InMemoryTemplateLoader() );
  m_engine->addTemplateLoader( loader );

  QString appDirPath = QFileInfo( QCoreApplication::applicationDirPath() ).absoluteDir().path();
  m_engine->setPluginPaths( QStringList() << QLatin1String( GRANTLEE_PLUGIN_PATH )
                                          << QString::fromLatin1( ":/plugins/" ) // For testtags.qs
                         );
  m_engine->addDefaultLibrary( QLatin1String( "grantlee_scriptabletags" ) );
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
  QFETCH( Grantlee::Error, error );

  Template t = m_engine->newTemplate( input, QLatin1String( QTest::currentDataTag() ) );

  Context context( dict );

  QString result = t->render( &context );

  if ( t->error() != NoError ) {
    if ( t->error() != error )
      qDebug() << t->errorString();
    QCOMPARE( t->error(), error );
    return;
  }

  // Didn't catch any errors, so make sure I didn't expect any.
  QCOMPARE( NoError, error );

  QCOMPARE( t->error(), NoError );

  QCOMPARE( result, output );
}

void TestLoaderTags::testIncludeTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;

  loader->setTemplate( QLatin1String( "basic-syntax01" ), QLatin1String( "Something cool" ) );
  loader->setTemplate( QLatin1String( "basic-syntax02" ), QLatin1String( "{{ headline }}" ) );

  QTest::newRow( "include01" ) << "{% include \"basic-syntax01\" %}" << dict << QString::fromLatin1( "Something cool" ) << NoError;

  dict.insert( QLatin1String( "headline" ), QLatin1String( "Included" ) );

  QTest::newRow( "include02" ) << "{% include \"basic-syntax02\" %}" << dict << QString::fromLatin1( "Included" ) << NoError;

  dict.insert( QLatin1String( "templateName" ), QLatin1String( "basic-syntax02" ) );

  QTest::newRow( "include03" ) << QString::fromLatin1( "{% include templateName %}" ) << dict << QString::fromLatin1( "Included" ) << NoError;

  dict.clear();
  QTest::newRow( "include04" ) << "a{% include \"nonexistent\" %}b" << dict << QString::fromLatin1( "ab" ) << TagSyntaxError;

  QString incl05 = QLatin1String( "template with a space" );
  loader->setTemplate( QLatin1String( "include 05" ), incl05 );

  QTest::newRow( "include 05" ) << incl05 << dict << QString::fromLatin1( "template with a space" ) << NoError;

  QTest::newRow( "include06" ) << "{% include \"include 05\" %}" << dict << QString::fromLatin1( "template with a space" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "list" ), QVariantList() << QVariant() << QVariant() );
  QTest::newRow( "include07" ) << "{% for i in list %}{% include \"include 05\" %}{% endfor %}" << dict << QString::fromLatin1( "template with a spacetemplate with a space" ) << NoError;

}


void TestLoaderTags::testExtendsTag_data()
{
  QTest::addColumn<QString>( "input" );
  QTest::addColumn<Dict>( "dict" );
  QTest::addColumn<QString>( "output" );
  QTest::addColumn<Grantlee::Error>( "error" );

  Dict dict;
  // Basic test
  QTest::newRow( "namedendblocks01" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock first %}3" ) << dict << QString::fromLatin1( "1_2_3" ) << NoError;
  // Unbalanced blocks
  QTest::newRow( "namedendblocks02" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock first %}_{% endblock second %}3" ) << dict << QString() << InvalidBlockTagError;
  QTest::newRow( "namedendblocks03" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock %}_{% endblock second %}3" ) << dict << QString() << InvalidBlockTagError;
  QTest::newRow( "namedendblocks04" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock third %}3" ) << dict << QString() << InvalidBlockTagError;
  QTest::newRow( "namedendblocks05" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock first %}" ) << dict << QString() << InvalidBlockTagError;
  // Mixed named and unnamed endblocks
  QTest::newRow( "namedendblocks06" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock %}_{% endblock first %}3" ) << dict << QString::fromLatin1( "1_2_3" ) << NoError;
  QTest::newRow( "namedendblocks07" ) << QString::fromLatin1( "1{% block first %}_{% block second %}2{% endblock second %}_{% endblock %}3" ) << dict << QString::fromLatin1( "1_2_3" ) << NoError;


  //## INHERITANCE ###########################################################
  // Standard template with no inheritance

  QString inh1 = QLatin1String( "1{% block first %}&{% endblock %}3{% block second %}_{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance01" ), inh1 );

  QTest::newRow( "inheritance01" ) << inh1 << dict << QString::fromLatin1( "1&3_" ) << NoError;

  QString inh2 = QLatin1String( "{% extends \"inheritance01\" %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance02" ), inh2 );

  // Standard two-level inheritance
  QTest::newRow( "inheritance02" ) << inh2 << dict << QString::fromLatin1( "1234" ) << NoError;
  // Three-level with no redefinitions on third level
  QTest::newRow( "inheritance03" ) << QString::fromLatin1( "{% extends 'inheritance02' %}" ) << dict << QString::fromLatin1( "1234" ) << NoError;
  // Two-level with no redefinitions on second level

  QString inh4 = QLatin1String( "{% extends \"inheritance01\" %}" );
  loader->setTemplate( QLatin1String( "inheritance04" ), inh4 );

  QTest::newRow( "inheritance04" ) << inh4 << dict << QString::fromLatin1( "1&3_" ) << NoError;
  // Two-level with double quotes instead of single quotes
  QTest::newRow( "inheritance05" ) << "{% extends \"inheritance02\" %}" << dict << QString::fromLatin1( "1234" ) << NoError;

  dict.insert( QLatin1String( "foo" ), QLatin1String( "inheritance02" ) );
  // Three-level with variable parent-template name
  QTest::newRow( "inheritance06" ) << QString::fromLatin1( "{% extends foo %}" ) << dict << QString::fromLatin1( "1234" ) << NoError;

  QString inh7 = QLatin1String( "{% extends 'inheritance01' %}{% block second %}5{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance07" ), inh7 );

  dict.clear();
  // Two-level with one block defined, one block not defined
  QTest::newRow( "inheritance07" ) << inh7 << dict << QString::fromLatin1( "1&35" ) << NoError;
  // Three-level with one block defined on this level, two blocks defined next level
  QTest::newRow( "inheritance08" ) << QString::fromLatin1( "{% extends 'inheritance02' %}{% block second %}5{% endblock %}" ) << dict << QString::fromLatin1( "1235" ) << NoError;

  // Three-level with second and third levels blank
  QTest::newRow( "inheritance09" ) << QString::fromLatin1( "{% extends 'inheritance04' %}" ) << dict << QString::fromLatin1( "1&3_" ) << NoError;

  dict.clear();

  // Three-level with space NOT in a block -- should be ignored
  QTest::newRow( "inheritance10" ) << QString::fromLatin1( "{% extends 'inheritance04' %}      " ) << dict << QString::fromLatin1( "1&3_" ) << NoError;
  // Three-level with both blocks defined on this level, but none on second level
  QTest::newRow( "inheritance11" ) << QString::fromLatin1( "{% extends 'inheritance04' %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" ) << dict << QString::fromLatin1( "1234" ) << NoError;
  // Three-level with this level providing one and second level providing the other
  QTest::newRow( "inheritance12" ) << QString::fromLatin1( "{% extends 'inheritance07' %}{% block first %}2{% endblock %}" ) << dict << QString::fromLatin1( "1235" ) << NoError;
  // Three-level with this level overriding second level
  QTest::newRow( "inheritance13" ) << QString::fromLatin1( "{% extends 'inheritance02' %}{% block first %}a{% endblock %}{% block second %}b{% endblock %}" ) << dict << QString::fromLatin1( "1a3b" ) << NoError;
  // A block defined only in a child template shouldn't be displayed
  QTest::newRow( "inheritance14" ) << QString::fromLatin1( "{% extends 'inheritance01' %}{% block newblock %}NO DISPLAY{% endblock %}" ) << dict << QString::fromLatin1( "1&3_" ) << NoError;


  QString inh15 = QLatin1String( "{% extends 'inheritance01' %}{% block first %}2{% block inner %}inner{% endblock %}{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance15" ), inh15 );

  // A block within another block
  QTest::newRow( "inheritance15" ) << inh15 << dict << QString::fromLatin1( "12inner3_" ) << NoError;
  // A block within another block (level 2)

  QTest::newRow( "inheritance16" ) << QString::fromLatin1( "{% extends 'inheritance15' %}{% block inner %}out{% endblock %}" ) << dict << QString::fromLatin1( "12out3_" ) << NoError;

  // {% load %} tag (parent -- setup for exception04)
  QString inh17 = QLatin1String( "{% load testtags %}{% block first %}1234{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance17" ), inh17 );

  dict.clear();
  QTest::newRow( "inheritance17" ) << inh17 << dict << QString::fromLatin1( "1234" ) << NoError;

  // {% load %} tag (standard usage, without inheritance)
  QTest::newRow( "inheritance18" ) << QString::fromLatin1( "{% load testtags %}{% echo this that theother %}5678" ) << dict << QString::fromLatin1( "this that theother5678" ) << NoError;

  // {% load %} tag (within a child template)
  QTest::newRow( "inheritance19" ) << QString::fromLatin1( "{% extends 'inheritance01' %}{% block first %}{% load testtags %}{% echo 400 %}5678{% endblock %}" ) << dict << QString::fromLatin1( "140056783_" ) << NoError;

  QString inh20 = QLatin1String( "{% extends 'inheritance01' %}{% block first %}{{ block.super }}a{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance20" ), inh20 );

  // Two-level inheritance with {{ block.super }}
  QTest::newRow( "inheritance20" ) << inh20 << dict << QString::fromLatin1( "1&a3_" ) << NoError;
  // Three-level inheritance with {{ block.super }} from parent
  QTest::newRow( "inheritance21" ) << QString::fromLatin1( "{% extends 'inheritance02' %}{% block first %}{{ block.super }}a{% endblock %}" ) << dict << QString::fromLatin1( "12a34" ) << NoError;
  // Three-level inheritance with {{ block.super }} from grandparent
  QTest::newRow( "inheritance22" ) << QString::fromLatin1( "{% extends 'inheritance04' %}{% block first %}{{ block.super }}a{% endblock %}" ) << dict << QString::fromLatin1( "1&a3_" ) << NoError;
  // Three-level inheritance with {{ block.super }} from parent and grandparent
  QTest::newRow( "inheritance23" ) << QString::fromLatin1( "{% extends 'inheritance20' %}{% block first %}{{ block.super }}b{% endblock %}" ) << dict << QString::fromLatin1( "1&ab3_" ) << NoError;

  // Inheritance from local context without use of template loader

  Template t = m_engine->newTemplate( QLatin1String( "1{% block first %}_{% endblock %}3{% block second %}_{% endblock %}" ), QLatin1String( "context_template" ) );
  dict.insert( QLatin1String( "context_template" ), QVariant::fromValue( t ) );

  QTest::newRow( "inheritance24" ) << QString::fromLatin1( "{% extends context_template %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" ) << dict << QString::fromLatin1( "1234" ) << NoError;

  dict.clear();
  QVariantList list;

  Template t1 = m_engine->newTemplate( QLatin1String( "Wrong" ), QLatin1String( "context_template_1" ) );
  Template t2 = m_engine->newTemplate( QLatin1String( "1{% block first %}_{% endblock %}3{% block second %}_{% endblock %}" ), QLatin1String( "context_template_2" ) );
  list << QVariant::fromValue( t1 );
  list << QVariant::fromValue( t2 );

  dict.insert( QLatin1String( "context_template" ), list );

  // Inheritance from local context with variable parent template
  QTest::newRow( "inheritance25" ) << QString::fromLatin1( "{% extends context_template.1 %}{% block first %}2{% endblock %}{% block second %}4{% endblock %}" ) << dict << QString::fromLatin1( "1234" ) << NoError;

  dict.clear();

  // Set up a base template to extend
  QString inh26 = QLatin1String( "no tags" );
  loader->setTemplate( QLatin1String( "inheritance26" ), inh26 );

  // Inheritance from a template that doesn't have any blocks
  QTest::newRow( "inheritance27" ) << QString::fromLatin1( "{% extends 'inheritance26' %}" ) << dict << QString::fromLatin1( "no tags" ) << NoError;

  QString inh28 = QLatin1String( "{% block first %}!{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance 28" ), inh28 );

  QTest::newRow( "inheritance 28" ) << inh28 << dict << QString::fromLatin1( "!" ) << NoError;

  // Inheritance from a template with a space in its name should work.
  QTest::newRow( "inheritance29" ) << QString::fromLatin1( "{% extends 'inheritance 28' %}" ) << dict << QString::fromLatin1( "!" ) << NoError;

  dict.insert( QLatin1String( "optional" ), QLatin1String( "True" ) );

  QString inh30 = QLatin1String( "1{% if optional %}{% block opt %}2{% endblock %}{% endif %}3" );
  loader->setTemplate( QLatin1String( "inheritance30" ), inh30 );

  QTest::newRow( "inheritance30" ) << inh30 << dict << QString::fromLatin1( "123" ) << NoError;
  QTest::newRow( "inheritance31" ) << QString::fromLatin1( "{% extends 'inheritance30' %}{% block opt %}two{% endblock %}" ) << dict << QString::fromLatin1( "1two3" ) << NoError;
  dict.clear();
  QTest::newRow( "inheritance32" ) << QString::fromLatin1( "{% extends 'inheritance30' %}{% block opt %}two{% endblock %}" ) << dict << QString::fromLatin1( "13" ) << NoError;

  dict.insert( QLatin1String( "optional" ), 1 );
  QString inh33 = QLatin1String( "1{% ifequal optional 1 %}{% block opt %}2{% endblock %}{% endifequal %}3" );
  loader->setTemplate( QLatin1String( "inheritance33" ), inh33 );

  QTest::newRow( "inheritance33" ) << inh33 << dict << QString::fromLatin1( "123" ) << NoError;

  QTest::newRow( "inheritance34" ) << QString::fromLatin1( "{% extends 'inheritance33' %}{% block opt %}two{% endblock %}" ) << dict << QString::fromLatin1( "1two3" ) << NoError;
  dict.clear();
  QTest::newRow( "inheritance35" ) << QString::fromLatin1( "{% extends 'inheritance33' %}{% block opt %}two{% endblock %}" ) << dict << QString::fromLatin1( "13" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "numbers" ), QVariantList() << 1 << 2 << 3 );

  QString inh36 = QLatin1String( "{% for n in numbers %}_{% block opt %}{{ n }}{% endblock %}{% endfor %}_" );
  loader->setTemplate( QLatin1String( "inheritance36" ), inh36 );

  QTest::newRow( "inheritance36" ) << inh36 << dict << QString::fromLatin1( "_1_2_3_" ) << NoError;
  QTest::newRow( "inheritance37" ) << QString::fromLatin1( "{% extends 'inheritance36' %}{% block opt %}X{% endblock %}" ) << dict << QString::fromLatin1( "_X_X_X_" ) << NoError;
  dict.clear();
  QTest::newRow( "inheritance38" ) << QString::fromLatin1( "{% extends 'inheritance36' %}{% block opt %}X{% endblock %}" ) << dict << QString::fromLatin1( "_" ) << NoError;

  dict.insert( QLatin1String( "optional" ), QLatin1String( "True" ) );

  QTest::newRow( "inheritance39" ) << QString::fromLatin1( "{% extends 'inheritance30' %}{% block opt %}new{{ block.super }}{% endblock %}" ) << dict << QString::fromLatin1( "1new23" ) << NoError;

  dict.insert( QLatin1String( "optional" ), 1 );

  QTest::newRow( "inheritance40" ) << QString::fromLatin1( "{% extends 'inheritance33' %}{% block opt %}new{{ block.super }}{% endblock %}" ) << dict << QString::fromLatin1( "1new23" ) << NoError;

  dict.clear();
  dict.insert( QLatin1String( "numbers" ), QVariantList() << 1 << 2 << 3 );

  QTest::newRow( "inheritance41" ) << QString::fromLatin1( "{% extends 'inheritance36' %}{% block opt %}new{{ block.super }}{% endblock %}" ) << dict << QString::fromLatin1( "_new1_new2_new3_" ) << NoError;

  QTest::newRow( "inheritance42" ) << QString::fromLatin1( "{% extends 'inheritance02'|cut:' ' %}" ) << dict << QString::fromLatin1( "1234" ) << NoError;

  /*
  // Tags that affect rendering have no afftect outside of a block in an extension template
  // (and therefore "i" is undefined here)
  // {% load %} tags for example are ok in child templates because they are processed at
  // compile-time, not at rendering time.
  QTest::newRow( "inheritance30" ) << QString::fromLatin1( "{% extends 'inheritance01' %}"
      "{% for i in list %}{% block first %}A{{ i }}B{% endblock %}{% endfor %}" ) << dict << QString::fromLatin1( "1AB3_" ) << NoError;

  QString inh31 = QLatin1String( "{% extends 'inheritance01' %}{% block first %}2{% block second %}4{% endblock %}{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance31" ), inh31 );

  // If blocks are siblings in a parent template and defined as children in an extension template,
  // the 'moved' block is rendered in its position in its parent template and in its position
  // in the extended template
  QTest::newRow( "inheritance31" ) << inh31 << dict << QString::fromLatin1( "12434" ) << NoError;

  QString inh32 = QLatin1String( "{% extends 'inheritance01' %}{% block second %}4{% block first %}2{% endblock %}{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance32" ), inh32 );

  // The order of appearance of the blocks does not determine this.
  QTest::newRow( "inheritance32" ) << inh32 << dict << QString::fromLatin1( "12342" ) << NoError;

  QTest::newRow( "inheritance33" ) << QString::fromLatin1( "{% extends 'inheritance01' %}"
      "{% block first %}2{% block second %}A{{ block.super }}B{% endblock %}{% endblock %}" ) << dict << QString::fromLatin1( "12AB3A_B" ) << NoError;

  QTest::newRow( "inheritance34" ) << QString::fromLatin1( "{% extends 'inheritance01' %}"
      "{% block second %}4{% block first %}A{{ block.super }}B{% endblock %}{% endblock %}" ) << dict << QString::fromLatin1( "1A&B34AB" ) << NoError;

  // Looping a block inside another block causes the block to be rendered multiple times. Once again,
  // When {{ i }} is rendered outside of the "first" block, "i" is not in context
  QTest::newRow( "inheritance35" ) << QString::fromLatin1( "{% extends 'inheritance01' %}"
      "{% block first %}"
      "A{% for i in list %}{% block second %}~{{ i }}:{% endblock %}{% endfor %}B"
      "{% endblock %}" ) << dict << QString::fromLatin1( "1A~One:~Two:~Three:B3~:" ) << NoError;

  QString inh36 = QLatin1String( "1{% block first %}&{% endblock %}3{% block second %}_{% endblock %}5{% block third %}+{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance36" ), inh36 );

  QTest::newRow( "inheritance36" ) << inh36 << dict << QString::fromLatin1( "1&3_5+" ) << NoError;

  QString inh37 = QLatin1String( "{% extends 'inheritance36' %}{% block second %}4{% block third %}6{% endblock %}{% endblock %}" );
  loader->setTemplate( QLatin1String( "inheritance37" ), inh37 );

  // The second contains the third:
  QTest::newRow( "inheritance37" ) << inh37 << dict << QString::fromLatin1( "1&34656" ) << NoError;

  // The first contains the second, contains the third:
  QTest::newRow( "inheritance38" ) << QString::fromLatin1( "{% extends 'inheritance37' %}"
      "{% block first %}2{% block second %}4{% block third %}6{% endblock %}{% endblock %}{% endblock %}" ) << dict << QString::fromLatin1( "124634656" ) << NoError;

  // ### Unexpected behaviour: we get "12AB3AABB56" here.
//  QTest::newRow( "inheritance39" ) << QString::fromLatin1( "{% extends 'inheritance37' %}" )
//      "{% block first %}2{% block second %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << QString::fromLatin1( "12A46B3A46B56" ) << NoError;

  // When {% block second %}{{ block.super }}{% endblock %} is rendered in the parent position (inheritance35)
  QTest::newRow( "inheritance40" ) << QString::fromLatin1( "{% extends 'inheritance37' %}"
      "{% block first %}2{% block third %}A{{ block.super }}B{% endblock %}{% endblock %}" ) << dict << QString::fromLatin1( "12AB34AB5AABB" ) << NoError;

  // ### Unexpected behaviour: we get "1&3A46ABB56AB" here
  // Actually, this is an infinite loop...
//  QTest::newRow( "inheritance41" ) << QString::fromLatin1( "{% extends 'inheritance37' %}" )
//      "{% block third %}6{% block second %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << QString::fromLatin1( "1&3A46B56A46B" ) << NoError;

  // ### Unexpected behaviour: we get "1A&B346AB56AB" here
//  QTest::newRow( "inheritance42" ) << QString::fromLatin1( "{% extends 'inheritance37' %}" )
//      "{% block third %}6{% block first %}A{{ block.super }}B{% endblock %}{% endblock %}" << dict << QString::fromLatin1( "1A&B346A&B56A&B" ) << NoError;

  QString inh43 = QLatin1String( "A{% block first %}B{% block second %}C{% endblock %}D{% endblock %}E" );
  loader->setTemplate( QLatin1String( "inheritance43" ), inh43 );

  QTest::newRow( "inheritance43" ) << inh43 << dict << QString::fromLatin1( "ABCDE" ) << NoError;

  QTest::newRow( "inheritance44" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block first %}1{% endblock %}" ) << dict << QString::fromLatin1( "A1E" ) << NoError;

  QTest::newRow( "inheritance45" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block first %}1{{ block.super }}2{% endblock %}" ) << dict << QString::fromLatin1( "A1BCD2E" ) << NoError;

  QTest::newRow( "inheritance46" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block second %}2{% endblock %}" ) << dict << QString::fromLatin1( "AB2DE" ) << NoError;

  QTest::newRow( "inheritance47" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block second %}2{{ block.super }}3{% endblock %}" ) << dict << QString::fromLatin1( "AB2C3DE" ) << NoError;

  // Although block second is overridden, it is not rendered because first is overridden excluding it.
  QTest::newRow( "inheritance48" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block first %}1{% endblock %}{% block second %}2{% endblock %}" ) << dict << QString::fromLatin1( "A1E" ) << NoError;

  // Block.super, which uses block second, renders the overridden block second.
  QTest::newRow( "inheritance49" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block first %}{{ block.super }}{% endblock %}{% block second %}2{% endblock %}" ) << dict << QString::fromLatin1( "AB2DE" ) << NoError;

  QTest::newRow( "inheritance50" ) << QString::fromLatin1( "{% extends 'inheritance43' %}"
      "{% block first %}{{ block.super }}{% endblock %}{% block second %}2{{ block.super }}3{% endblock %}" ) << dict << QString::fromLatin1( "AB2C3DE" ) << NoError;

  // ### Unexpected result: We get A1234E here. block.super doesn't work if overriding in a nested block I guess.
//  QTest::newRow( "inheritance51" ) << QString::fromLatin1( "{% extends 'inheritance43' %}" )
//      "{% block first %}1{% block second %}2{{ block.super }}3{% endblock %}4{% endblock %}" << dict << QString::fromLatin1( "A12C34E" ) << NoError;
*/
  dict.clear();
  // Raise exception for invalid template name
  QTest::newRow( "exception01" ) << QString::fromLatin1( "{% extends 'nonexistent' %}" ) << dict << QString() << TagSyntaxError;
  // Raise exception for invalid template name (in variable)
  QTest::newRow( "exception02" ) << QString::fromLatin1( "{% extends nonexistent %}" ) << dict << QString() << TagSyntaxError;
  // Raise exception for extra {% extends %} tags
  QTest::newRow( "exception03" ) << QString::fromLatin1( "{% extends 'inheritance01' %}{% block first %}2{% endblock %}{% extends 'inheritance16' %}" ) << dict << QString() << TagSyntaxError;
  // Raise exception for custom tags used in child with {% load %} tag in parent, not in child
  QTest::newRow( "exception04" ) << QString::fromLatin1( "{% extends 'inheritance17' %}{% block first %}{% echo 400 %}5678{% endblock %}" ) << dict << QString() << InvalidBlockTagError;
}


QTEST_MAIN( TestLoaderTags )
#include "testloadertags.moc"

#endif

