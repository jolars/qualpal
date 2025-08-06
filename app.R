library(shiny)
library(bslib)
library(qualpalr)
library(jsonlite)
library(maps)
library(colourpicker)

options(shiny.autoreload = TRUE)
# Copy the compiled css theme to your project folder.
file.copy(precompiled_css_path(theme = bs_theme()), "./bootstrap.min.css")

ui <- page_fillable(
  title = "Qualpal",
  theme = "bootstrap.min.css",
  div(
    style = "max-width: 1400px; margin: 0 auto; padding: 20px; width: 100%;",
    div(
      style = "display: grid; grid-template-columns: 300px 1fr; gap: 20px; width: 100%;",
      div(
        style = "background-color: #f8f9fa; padding: 20px; border-radius: 8px; margin-bottom: 20px; height: fit-content; position: sticky; top: 20px;",
        h3("Palette Controls", style = "margin-top: 0;"),
        numericInput(
          inputId = "palette_size",
          label = "Size",
          min = 1,
          max = 20,
          value = 5
        ),
        sliderInput(
          inputId = "hue",
          label = "Hue",
          min = 0,
          max = 360,
          value = c(30, 210),
          step = 1
        ),
        sliderInput(
          inputId = "saturation",
          label = "Saturation",
          min = 0,
          max = 1,
          value = c(0.2, 0.8)
        ),
        sliderInput(
          inputId = "lightness",
          label = "Lightness",
          min = 0,
          max = 1,
          value = c(0.2, 0.8)
        ),
        checkboxInput(
          inputId = "use_background_color",
          label = "Consider Background Color",
          value = FALSE
        ),
        conditionalPanel(
          condition = "input.use_background_color",
          colourInput(
            inputId = "background_color",
            label = "Background Color",
            value = "#ffffff",
            showColour = "both"
          )
        ),
        h4("Color Vision Deficiency"),
        p(
          "Simulate color vision deficiency to see how your palette appears to users with different types of color blindness."
        ),
        sliderInput(
          inputId = "protanopia",
          label = "Protanopia",
          min = 0,
          max = 1,
          value = 0,
          step = 0.05
        ),
        sliderInput(
          inputId = "deuteranopia",
          label = "Deuteranopia",
          min = 0,
          max = 1,
          value = 0,
          step = 0.05
        ),
        sliderInput(
          inputId = "tritanopia",
          label = "Tritanopia",
          min = 0,
          max = 1,
          value = 0,
          step = 0.05
        )
      ),
      div(
        style = "overflow-x: auto; min-width: 0;",
        h4("Generated Color Palette"),
        uiOutput(outputId = "colorPalette"),
        br(),
        div(
          style = "margin-top: 40px;",
          h4("Examples"),
          div(
            style = "display: grid; grid-template-columns: 1fr 1fr; gap: 20px;",
            plotOutput("examplePlot1", height = "300px"),
            plotOutput("examplePlot2", height = "300px")
          ),
          div(
            style = "display: grid; grid-template-columns: 1fr 1fr; gap: 20px; margin-top: 20px;",
            plotOutput("examplePlot3", height = "300px"),
            plotOutput("examplePlot4", height = "300px"),
            plotOutput("examplePlot5", height = "300px")
          )
        ),
        br(),
        div(
          style = "margin-top: 30px;",
          h4("Export Formats"),
          tabsetPanel(
            tabPanel(
              "CSS Variables",
              div(
                style = "margin-top: 15px;",
                verbatimTextOutput("cssOutput")
              )
            ),
            tabPanel(
              "JSON",
              div(
                style = "margin-top: 15px;",
                verbatimTextOutput("jsonOutput")
              )
            ),
            tabPanel(
              "R Vector",
              div(
                style = "margin-top: 15px;",
                verbatimTextOutput("rOutput")
              )
            )
          )
        )
      )
    )
  )
)

server <- function(input, output) {
  palette <- reactive({
    qualpal(
      input$palette_size,
      list(
        h = input$hue,
        s = input$saturation,
        l = input$lightness
      ),
      bg = if (input$use_background_color) {
        input$background_color
      } else {
        NULL
      }
    )
  })

  output$colorPalette <- renderUI({
    pal <- palette()

    color_blocks <- lapply(pal$hex, function(color) {
      # Convert hex to RGB
      rgb_vals <- col2rgb(color)
      rgb_text <- sprintf(
        "RGB(%d, %d, %d)",
        rgb_vals[1],
        rgb_vals[2],
        rgb_vals[3]
      )

      div(
        style = "display: inline-block; margin: 10px; text-align: center; vertical-align: top;",
        div(
          style = sprintf(
            "width: 100px; height: 80px; background-color: %s; border: 1px solid #ccc; border-radius: 6px; margin-bottom: 1ex;",
            color
          )
        ),
        div(
          style = "font-family: monospace; font-size: 12px; line-height: 1.3; width: 100px;",
          div(style = "font-weight: bold;", color),
          div(style = "color: #666;", rgb_text)
        )
      )
    })

    output$cssOutput <- renderText({
      pal <- palette()
      css_vars <- paste0("  --color-", seq_along(pal$hex), ": ", pal$hex, ";")
      paste(":root {", paste(css_vars, collapse = "\n"), "}", sep = "\n")
    })

    # JSON output
    output$jsonOutput <- renderText({
      pal <- palette()
      jsonlite::toJSON(list(colors = pal$hex), pretty = TRUE, auto_unbox = TRUE)
    })

    # R Vector output
    output$rOutput <- renderText({
      pal <- palette()
      paste0('c("', paste(pal$hex, collapse = '", "'), '")')
    })

    div(
      style = "text-align: center; padding: 20px;",
      color_blocks
    )
  })

  # Example plots
  output$examplePlot1 <- renderPlot({
    pal <- palette()
    data <- data.frame(
      category = LETTERS[seq_len(length(pal$hex))],
      value = runif(length(pal$hex), 10, 100)
    )

    barplot(
      data$value,
      names.arg = data$category,
      col = pal$hex,
      las = 2,
      cex.names = 0.8
    )
  })

  output$examplePlot2 <- renderPlot({
    pal <- palette()
    values <- runif(min(length(pal$hex), 8), 10, 30)
    pie(
      values,
      labels = LETTERS[seq_along(values)],
      col = pal$hex[seq_along(values)]
    )
  })

  output$examplePlot3 <- renderPlot({
    pal <- palette()
    n_points <- min(length(pal$hex), 6)
    data <- data.frame(
      x = runif(n_points * 20),
      y = runif(n_points * 20),
      group = rep(1:n_points, each = 20)
    )

    plot(
      data$x,
      data$y,
      col = pal$hex[data$group],
      pch = 19,
      xlab = "x",
      ylab = "y",
      frame.plot = TRUE
    )
  })

  output$examplePlot4 <- renderPlot({
    pal <- palette()

    x <- 1:50
    n_lines <- length(pal$hex)
    plot(
      x,
      sin(x / 5),
      type = "l",
      col = pal$hex[1],
      xlab = "x",
      ylab = "y",
      lwd = 2
    )

    for (i in 2:n_lines) {
      lines(x, sin(x / 5 + i), col = pal$hex[i], lwd = 2)
    }
  })

  output$examplePlot5 <- renderPlot({
    pal <- palette()

    # Get US state names
    state_names <- map("state", namesonly = TRUE, plot = FALSE)
    n_groups <- min(length(pal$hex), length(state_names))

    # Assign states to groups cyclically to distribute evenly
    state_colors <- rep(
      pal$hex[seq_len(n_groups)],
      length.out = length(state_names)
    )

    # Create the map
    par(mar = c(1, 1, 1, 1))
    map(
      "state",
      col = state_colors,
      fill = TRUE,
      border = "white",
      lwd = 0.5,
      main = "US States Map Example"
    )
  })
}

shinyApp(ui = ui, server = server)
