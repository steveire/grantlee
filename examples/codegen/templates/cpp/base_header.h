{% include licence|append:".h" %}

{% random 5 as rand %}
#ifndef {{ className.upper }}_H_{{ rand }}
#define {{ className.upper }}_H_{{ rand }}

{% block includes %}
{% endblock %}
{% block forward_decls %}
{% endblock %}
{% block apidox %}
/**
  @brief The {{ className }} class

  @author {{ author }}
  @date {% now "MMM yyyy" %}
*/
{% endblock %}
class {{ className }}{% if baseClass %} : {{ baseClass.access }} {{ baseClass.type }} {% endif %}
{
{% block class_content %}
{% endblock %}
};

#endif // {{ className.upper }}_H_{{ rand }}
