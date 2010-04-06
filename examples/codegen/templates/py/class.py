#! /usr/bin/env python
# -*- coding: utf-8 -*-

{% include licence|append:".py" %}

{% if baseClass %}from PyQt4.{{ baseClass.module }} import {{ baseClass.type }}{% endif %}

class {{ className }}({% if baseClass %}{{ baseClass.type }}{% else %}object{% endif %}):

    def __init__(self):
        {% if baseClass %}{{ baseClass.type }}.__init__(self){% endif %}

    {% for property in properties %}
    def {{ property.name }}(self):
        return self._{{ property.name }}
    {% if not property.readonly %}
    def {{ property.name|to_write }}(self, {{ property.name }}):
        self._{{ property.name }} = {{ property.name }}
    {% endif %}{% endfor %}
    {% with "true" as default %}{% for method in methods %}
    def {{ method.name }}(self{% include "args.py" %}):
        pass
      {% endfor %}{% endwith %}

