{% extends "base_header.h" %}

{% block includes %}
#include "{{ className|to_public|lower }}.h"
{% endblock %}

{# override the apidox block with nothing #}
{% block apidox %}

{% endblock %}

{% block class_content %}
  {{ className }}Private( {{ className|to_public }} *qq )
    : q_ptr(qq)
  {

  }
  Q_DECLARE_PUBLIC({{ className|to_public }})
  {{ className|to_public  }} * const q_ptr;

{% for property in properties %}
  {{ property.type }} m_{{ property.name }};
{% endfor %}
{% endblock %}
