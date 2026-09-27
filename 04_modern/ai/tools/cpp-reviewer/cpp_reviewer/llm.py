from __future__ import annotations

import json
import os
from typing import Any

from openai import OpenAI


REVIEW_SCHEMA: dict[str, Any] = {
    "type": "object",
    "additionalProperties": False,
    "properties": {
        "summary": {
            "type": "string",
        },
        "findings": {
            "type": "array",
            "items": {
                "type": "object",
                "additionalProperties": False,
                "properties": {
                    "severity": {
                        "type": "string",
                        "enum": [
                            "HIGH",
                            "MEDIUM",
                            "LOW",
                            "INFO",
                        ],
                    },
                    "confidence": {
                        "type": "string",
                        "enum": [
                            "HIGH",
                            "MEDIUM",
                            "LOW",
                        ],
                    },
                    "category": {
                        "type": "string",
                    },
                    "title": {
                        "type": "string",
                    },
                    "file": {
                        "type": "string",
                    },
                    "line": {
                        "type": "integer",
                    },
                    "evidence": {
                        "type": "string",
                    },
                    "explanation": {
                        "type": "string",
                    },
                    "recommendation": {
                        "type": "string",
                    },
                },
                "required": [
                    "severity",
                    "confidence",
                    "category",
                    "title",
                    "file",
                    "line",
                    "evidence",
                    "explanation",
                    "recommendation",
                ],
            },
        },
    },
    "required": [
        "summary",
        "findings",
    ],
}


class LLMReviewer:
    def __init__(self) -> None:
        api_key = os.getenv("OPENAI_API_KEY")

        if not api_key:
            raise RuntimeError(
                "OPENAI_API_KEY is not set."
            )

        self.model = os.getenv(
            "OPENAI_MODEL",
            "gpt-5.6-sol",
        )

        self.client = OpenAI(
            api_key=api_key
        )

    def review(
        self,
        system_prompt: str,
        user_prompt: str,
    ) -> dict[str, Any]:
        response = self.client.responses.create(
            model=self.model,
            instructions=system_prompt,
            input=user_prompt,
            text={
                "format": {
                    "type": "json_schema",
                    "name": "cpp_code_review",
                    "strict": True,
                    "schema": REVIEW_SCHEMA,
                }
            },
        )

        return json.loads(
            response.output_text
        )
