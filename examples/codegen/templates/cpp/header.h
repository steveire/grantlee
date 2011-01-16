{% extends "base_header.h" %}

{% block includes %}
  {% if baseClass %}
#include <{{ baseClass.module }}/{{ baseClass.type }}>

  {% endif %}
{% endblock %}

{% block forward_decls %}
  {% if pimpl %}
class {{ className }}Private;

  {% endif %}
{% endblock %}

{% block class_content %}
  {% if qobject %}
  Q_OBJECT
  {% endif %}
  {% for prop in properties %}
  Q_PROPERTY({{ prop.type }} {{ prop.name }} READ {{ prop.name }}{% if not prop.readonly %} WRITE {{ prop.name|to_write }}{% endif %})
  {% endfor %}
public:
  {# The getter, eg QString myString() const; #}
    {% for property in properties %}
  {{ property.type }} {{ property.name }}() const;
    {% if not property.readonly %}
      {% comment %}
      The setter, eg setMyString(const QString &myString);
      {% endcomment %}
  void {{ property.name|to_write }}({{ property.type|to_arg }}{{ property.name }});
    {% endif %}

  {% endfor %}
  {% comment %}
    Comment tags are used to remove unwanted whitespace/newlines in the output.

    Group together properties of the same type, public, protected, private
  {% endcomment %}
  {% regroup methods by accessType as method_groups %}
  {% with "true" as default %}
    {% for method_group in method_groups %}
{{ method_group.grouper }}:
      {% for method in method_group.list %}
  {% if method.virtual %}virtual {% endif %}{{ method.type }} {{ method.name }}({% include "args.h" %}){% if method.const %} const{% endif %};
      {% endfor %}

    {% endfor %}
  {% endwith %}
private:
  {% if pimpl %}
  Q_DECLARE_PRIVATE({{ className }})
  {{ className }}Private * const d_ptr;
  {% else %}
    {% for property in properties %}
  {{ property.type }} m_{{ property.name }};
    {% endfor %}
  {% endif %}
{% endblock class_content %}
