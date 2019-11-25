apps += External::App
app_headers += apps/external/app.h

app_external_src = $(addprefix apps/external/,\
	api.cpp \
	app.cpp \
	archive.cpp \
	main_controller.cpp \
	pointer_text_table_cell.cpp \
)

app_src += $(app_external_src)

i18n_files += $(addprefix apps/external/,\
  base.de.i18n\
  base.en.i18n\
  base.es.i18n\
  base.fr.i18n\
  base.pt.i18n\
)

$(eval $(call depends_on_image,apps/external/app.cpp,apps/external/external_icon.png))
